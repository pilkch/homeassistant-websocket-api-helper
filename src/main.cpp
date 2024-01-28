#include <fstream>
#include <iostream>
#include <string>  

#include <curl/curl.h>

#include "curl_helper.h"
#include "settings.h"
#include "util.h"
#include "web_socket.h"

const std::string URL = "wss://homeassistant.network.home:8443/api/websocket";

void do_websocket_homeassistant(curl::cWebSocket& ws)
{
  char buffer[2 * 1024] = {0};

  util::msleep(20);

  const ssize_t nbytes_read = ws.Receive(buffer, sizeof(buffer));
  if (nbytes_read == 0) {
    std::cout<<"Try again later"<<std::endl;
  } else if (nbytes_read < 0) {
    std::cout<<"Receive returned 0"<<std::endl;
  } else {
    std::cout<<"Receive received data: \""<<std::string(buffer, nbytes_read)<<"\""<<std::endl;
  }

  util::msleep(100);

  ws.SendClose();
}

int main()
{
  application::cSettings settings;
  if (!settings.LoadFromFile("./configuration.json")) {
    std::cerr<<"Error parsing configuration.json"<<std::endl;
    return EXIT_FAILURE;
  }

  curl::cCurlHelper helper;

  curl::cWebSocket ws;
  if (!ws.Open(URL, "./homeassistant.network.home.crt")) {
    std::cerr<<"Error opening web socket"<<std::endl;
    return EXIT_FAILURE;
  }

  do_websocket_homeassistant(ws);

  return EXIT_SUCCESS;
}
