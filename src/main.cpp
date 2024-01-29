#include <iostream>

#include "curl_helper.h"
#include "home_assistant.h"
#include "settings.h"
#include "web_socket.h"

const std::string URL = "wss://homeassistant.network.home:8443/api/websocket";

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

  if (!homeassistant::Authenticate(ws, settings)) {
    std::cerr<<"Authentication failed"<<std::endl;
    return EXIT_FAILURE;
  }

  const std::string backup_hash = homeassistant::CreateBackup(ws, settings);
  if (backup_hash.empty()) {
    std::cerr<<"Error creating backup"<<std::endl;
    return EXIT_FAILURE;
  }

  if (!homeassistant::DownloadBackup(ws, settings, backup_hash)) {
    std::cerr<<"Error downloading backup"<<std::endl;
    return EXIT_FAILURE;
  }

  ws.SendClose();

  return EXIT_SUCCESS;
}
