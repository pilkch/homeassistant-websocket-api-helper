#include <iostream>
#include <string>

#include <curl/curl.h>

#include "curl_helper.h"
#include "web_socket.h"

const std::string URL = "wss://ws.postman-echo.com/raw";


int main()
{
  curl::curl_helper helper;

  curl::web_socket ws;
  if (!ws.open(URL)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
