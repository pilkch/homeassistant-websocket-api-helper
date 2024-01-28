#include <fstream>
#include <iostream>
#include <string>

#include "home_assistant.h"
#include "util.h"

namespace homeassistant {

std::string CreateBackup(curl::cWebSocket& ws, const application::cSettings& settings)
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

  return "";
}

bool DownloadBackup(curl::cWebSocket& ws, const application::cSettings& settings, const std::string& backup_hash)
{
  (void)ws;
  (void)settings;
  (void)backup_hash;

  return false;
}

}
