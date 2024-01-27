#include <iostream>

#include "web_socket.h"

// Based on this awesome example by Ben Wills
// https://github.com/benwills/libcurl-websocket-example-basic/blob/main/test.curl.c

namespace curl {

web_socket::web_socket() :
  curl(nullptr)
{
}

web_socket::~web_socket()
{
  close();
}

bool web_socket::open(std::string_view url)
{
  close();

  curl = curl_easy_init();
  if (curl == nullptr) {
    return false;
  }

  curl_easy_setopt(curl, CURLOPT_URL, url.data());
  curl_easy_setopt(curl, CURLOPT_USERAGENT, url.data());
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // Turn on debugging
  curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L); // Websocket style
  const CURLcode res = curl_easy_perform(curl);

  const CURLcode result = curl_easy_perform(curl);

  return (result == CURLE_OK);
}

void web_socket::close()
{
  curl_easy_cleanup(curl);
  curl = nullptr;
}

ssize_t web_socket::send(std::string_view message)
{
  size_t nbytes_written = 0;
  const CURLcode result = curl_ws_send(curl, message.data(), message.length(), &nbytes_written, 0, CURLWS_TEXT);
  if (result == CURLE_OK) {
    return ssize_t(nbytes_written);
  }

  return -1;
}

bool web_socket::send_close()
{
  size_t sent;
  CURLcode result = curl_ws_send(curl, "", 0, &sent, 0, CURLWS_CLOSE);
  return (result == CURLE_OK);
}

ssize_t web_socket::receive(char* data, size_t data_length)
{
  size_t nbytes_read = 0;
  const struct curl_ws_frame* pMeta = nullptr;
  const CURLcode result = curl_ws_recv(curl, data, data_length, &nbytes_read, &pMeta);
  if (result == CURLE_OK) {
    //std::cout<<"web_socket::receive nread "<<nbytes_read<<" Age "<<pMeta->age<<" Flags "<<pMeta->flags<<" Offset "<<pMeta->offset<<" Bytesleft "<<pMeta->bytesleft<<std::endl;
    return ssize_t(nbytes_read);
  } else if (result == CURLE_AGAIN) {
    //std::cout<<"web_socket::receive Try again later"<<std::endl;
    return 0;
  }

  return -1;
}

}
