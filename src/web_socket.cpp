#include <iostream>

#include "web_socket.h"

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
  size_t nbytes_read = 0;
  const struct curl_ws_frame* pMeta = nullptr;
  const CURLcode result = curl_ws_recv(curl, data, data_length, &nbytes_read, &pMeta);
  if (result == CURLE_OK) {
    //std::cout<<"cWebSocket::Receive nread "<<nbytes_read<<" Age "<<pMeta->age<<" Flags "<<pMeta->flags<<" Offset "<<pMeta->offset<<" Bytesleft "<<pMeta->bytesleft<<std::endl;
    return ssize_t(nbytes_read);
  } else if (result == CURLE_AGAIN) {
    //std::cout<<"cWebSocket::Receive Try again later"<<std::endl;
    return 0;
  }

  return -1;
}

}
