#pragma once

#include "settings.h"
#include "web_socket.h"

namespace homeassistant {

// Returns the backup hash if a backup was created, or an empty string
std::string CreateBackup(curl::cWebSocket& ws, const application::cSettings& settings);

// Downloads an already created backup
bool DownloadBackup(curl::cWebSocket& ws, const application::cSettings& settings, const std::string& backup_hash);

}
