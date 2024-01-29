#include <iostream>

#include "web_socket.h"

namespace {

// Auxiliary function that waits on the socket
// From: https://curl.se/libcurl/c/sendrecv.html
int wait_on_socket(curl_socket_t read_sockfd, long timeout_ms)
{
  struct timeval tv;
  tv.tv_sec = timeout_ms / 1000;
  tv.tv_usec = (int)(timeout_ms % 1000) * 1000;
 
  fd_set infd;
  FD_ZERO(&infd);
  fd_set errfd;
  FD_ZERO(&errfd);
 
  FD_SET(read_sockfd, &errfd); // Always check for errors
  FD_SET(read_sockfd, &infd);
 
  const int maxfd = int(read_sockfd) + 1;

  // select() returns the number of signalled sockets or -1
  return select(maxfd, &infd, nullptr, &errfd, &tv);
}

}

// Based on this awesome example by Ben Wills
// https://github.com/benwills/libcurl-websocket-example-basic/blob/main/test.curl.c

namespace curl {

cWebSocket::cWebSocket() :
  curl(nullptr)
{
}

cWebSocket::~cWebSocket()
{
  Close();
}

bool cWebSocket::Open(std::string_view url, const std::optional<std::string>& self_signed_certificate_path)
{
  Close();

  curl = curl_easy_init();
  if (curl == nullptr) {
    return false;
  }

  curl_easy_setopt(curl, CURLOPT_URL, url.data());
  curl_easy_setopt(curl, CURLOPT_USERAGENT, url.data());
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // Turn on debugging
  curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L); // Websocket style

  // Check if we need to provide a self signed public key
  if (self_signed_certificate_path) {
    curl_easy_setopt(curl, CURLOPT_CAINFO, self_signed_certificate_path.value().c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1);
  }

  const CURLcode result = curl_easy_perform(curl);

  return (result == CURLE_OK);
}

void cWebSocket::Close()
{
  curl_easy_cleanup(curl);
  curl = nullptr;
}

ssize_t cWebSocket::Send(std::string_view message)
{
  size_t nbytes_written = 0;
  const CURLcode result = curl_ws_send(curl, message.data(), message.length(), &nbytes_written, 0, CURLWS_TEXT);
  if (result == CURLE_OK) {
    return ssize_t(nbytes_written);
  }

  return -1;
}

bool cWebSocket::SendClose()
{
  size_t sent;
  CURLcode result = curl_ws_send(curl, "", 0, &sent, 0, CURLWS_CLOSE);
  return (result == CURLE_OK);
}

ssize_t cWebSocket::Receive(char* data, size_t data_length)
{
  // Curl seems to receive up to 16384 bytes at a time, so we may need to read multiple times to receive everything

  size_t ntotal_bytes_read = 0;

  while (data_length != 0) {
    size_t nbytes_read = 0;
    const struct curl_ws_frame* pMeta = nullptr;
    const CURLcode result = curl_ws_recv(curl, data, data_length, &nbytes_read, &pMeta);
    if (result == CURLE_OK) {
      std::cout<<"cWebSocket::Receive nread "<<nbytes_read<<" Age "<<pMeta->age<<" Flags "<<pMeta->flags<<" Offset "<<pMeta->offset<<" Bytesleft "<<pMeta->bytesleft<<std::endl;
      ntotal_bytes_read += nbytes_read;

      data += nbytes_read;
      data_length -= nbytes_read;

      // No bytes left to read, may as well return
      if (pMeta->bytesleft == 0) {
        break;
      }
    } else if (result == CURLE_AGAIN) {
      //std::cout<<"cWebSocket::Receive Try again later"<<std::endl;
      break;
    } else {
      return -1;
    }
  }

  return ntotal_bytes_read;
}

ssize_t cWebSocket::Receive(char* data, size_t data_length, uint32_t timeout_ms)
{
  // https://curl.se/libcurl/c/sendrecv.html

  // Try to read
  const size_t ntotal_bytes_read = Receive(data, data_length);
  if (ntotal_bytes_read > 0) {
    return ntotal_bytes_read;
  }

  // If that didn't work then wait for the socket to be writeable or the timeout
  // Get the socket
  curl_socket_t sockfd;
  CURLcode curl_result = curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sockfd);
  if (curl_result != CURLE_OK) {
    std::cout<<"Error getting socket: "<<curl_easy_strerror(curl_result)<<std::endl;
    return -1;
  }

  const int result = wait_on_socket(sockfd, timeout_ms);
  if (result != 1) {
    return -1;
  }

  // The socket is now ready so try reading again
  return Receive(data, data_length);
}

}
