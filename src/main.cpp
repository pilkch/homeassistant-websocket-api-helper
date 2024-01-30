#include <iostream>

#include <sysexits.h>

#include "curl_helper.h"
#include "home_assistant.h"
#include "settings.h"
#include "version.h"
#include "web_socket.h"

namespace application {

void PrintUsage()
{
  std::cout<<"Usage: ./homeassistant-websocket-api-helper [OPTION]"<<std::endl;
  std::cout<<std::endl;
  std::cout<<"  -v, --version                 Print the version and exit"<<std::endl;
  std::cout<<"  --create-and-download-backup  Create a backup and download it to a tar file in the current directory"<<std::endl;
}

void PrintVersion()
{
  std::cout<<"homeassistant-websocket-api-helper version "<<version<<std::endl;
}

}

int main(int argc, char* argv[])
{
  if (argc != 2) {
    // Incorrect number of arguments, print the usage and exit
    application::PrintUsage();
    return EX_USAGE;
  }

  const std::string argument(argv[1]);
  if ((argument == "-v") || (argument == "--version")) {
    application::PrintVersion();
    return EXIT_SUCCESS;
  } else if ((argument != "--create-and-download-backup")) {
    // Unknown argument, print the usage and exit
    application::PrintUsage();
    return EX_USAGE;
  }

  // Must be "--create-and-download-backup"

  // Parse the configuration file
  application::cSettings settings;
  if (!settings.LoadFromFile("./configuration.json")) {
    std::cerr<<"Error parsing configuration.json"<<std::endl;
    return EXIT_FAILURE;
  }

  // Create a backup and download it
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
