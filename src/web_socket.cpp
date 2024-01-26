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

  return (res == CURLE_OK);
}

void web_socket::close()
{
  curl_easy_cleanup(curl);
  curl = nullptr;
}

}
