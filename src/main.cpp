#include <iostream>

#include "curl_helper.h"
#include "home_assistant.h"
#include "settings.h"
#include "web_socket.h"

int main()
{
  application::cSettings settings;
  if (!settings.LoadFromFile("./configuration.json")) {
    std::cerr<<"Error parsing configuration.json"<<std::endl;
    return EXIT_FAILURE;
  }

  {
    curl::cCurlHelper helper;

    std::string backup_hash;

    // Create a backup with the web socket API
    {
      const std::string URL = "wss://" + settings.GetHostName() + ":" + std::to_string(settings.GetPort()) + "/api/websocket";

      curl::cWebSocket ws;
      if (!ws.Open(URL, settings.GetSelfSignedCertificate())) {
        std::cerr<<"Error opening web socket"<<std::endl;
        return EXIT_FAILURE;
      }

      if (!homeassistant::Authenticate(ws, settings)) {
        std::cerr<<"Authentication failed"<<std::endl;
        return EXIT_FAILURE;
      }

      backup_hash = homeassistant::CreateBackup(ws);
      if (backup_hash.empty()) {
        std::cerr<<"Error creating backup"<<std::endl;
        return EXIT_FAILURE;
      }

      ws.SendClose();
    }

    std::cout<<"*************************************************************"<<std::endl;

    // Download the backup with the regular HTTP API
    if (!homeassistant::DownloadBackup(settings, backup_hash)) {
      std::cerr<<"Error downloading backup"<<std::endl;
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
