#include <fstream>
#include <iostream>
#include <string>

#include "json.h"
#include "home_assistant.h"
#include "util.h"

namespace {

// {
//   "type": "auth_required",
//   "ha_version": "2021.5.3"
// }
bool ParseAuthRequired(const std::string& json)
{
  util::cJSONDocument document(json_tokener_parse(json.c_str()));
  if (!document.IsValid()) {
    std::cerr<<"ParseAuthRequired Invalid JSON, returning false"<<std::endl;
    return false;
  }

  // Parse type
  {
    struct json_object* type_obj = json_object_object_get(document.Get(), "type");
    if (type_obj == nullptr) {
      std::cerr<<"ParseAuthRequired type not found"<<std::endl;
      return false;
    }

    // Make sure the type is a string
    if (json_object_get_type(type_obj) != json_type_string) {
      std::cerr<<"ParseAuthRequired type is not a string"<<std::endl;
      return false;
    }

    const char* value = json_object_get_string(type_obj);
    if (value == nullptr) {
      std::cerr<<"ParseAuthRequired Error getting type value"<<std::endl;
      return false;
    }

    const std::string type = value;
    if (type != "auth_required") {
      std::cerr<<"ParseAuthRequired Invalid type \""<<type<<"\""<<std::endl;
      return false;
    }
  }

  // Parse ha_version
  {
    struct json_object* ha_version_obj = json_object_object_get(document.Get(), "ha_version");
    if (ha_version_obj == nullptr) {
      std::cerr<<"ParseAuthRequired ha_version not found"<<std::endl;
      return false;
    }

    // Make sure the ha_version is a string
    if (json_object_get_type(ha_version_obj) != json_type_string) {
      std::cerr<<"ParseAuthRequired ha_version is not a string"<<std::endl;
      return false;
    }

    const char* value = json_object_get_string(ha_version_obj);
    if (value == nullptr) {
      std::cerr<<"ParseAuthRequired Error getting ha_version value"<<std::endl;
      return false;
    }

    const std::string ha_version = value;
    if (ha_version.empty()) {
      std::cerr<<"ParseAuthRequired Invalid ha_version \""<<ha_version<<"\""<<std::endl;
      return false;
    }
  }

  return true;
}

// {
//   "type": "auth_ok",
//   "ha_version": "2021.5.3"
// }
bool ParseAuthOk(const std::string& json)
{
  util::cJSONDocument document(json_tokener_parse(json.c_str()));
  if (!document.IsValid()) {
    std::cerr<<"ParseAuthOk Invalid JSON, returning false"<<std::endl;
    return false;
  }

  // Parse type
  {
    struct json_object* type_obj = json_object_object_get(document.Get(), "type");
    if (type_obj == nullptr) {
      std::cerr<<"ParseAuthOk type not found"<<std::endl;
      return false;
    }

    // Make sure the type is a string
    if (json_object_get_type(type_obj) != json_type_string) {
      std::cerr<<"ParseAuthOk type is not a string"<<std::endl;
      return false;
    }

    const char* value = json_object_get_string(type_obj);
    if (value == nullptr) {
      std::cerr<<"ParseAuthOk Error getting type value"<<std::endl;
      return false;
    }

    const std::string type = value;
    if (type != "auth_ok") {
      std::cerr<<"ParseAuthOk Invalid type \""<<type<<"\""<<std::endl;
      return false;
    }
  }

  // Parse ha_version
  {
    struct json_object* ha_version_obj = json_object_object_get(document.Get(), "ha_version");
    if (ha_version_obj == nullptr) {
      std::cerr<<"ParseAuthOk ha_version not found"<<std::endl;
      return false;
    }

    // Make sure the ha_version is a string
    if (json_object_get_type(ha_version_obj) != json_type_string) {
      std::cerr<<"ParseAuthOk ha_version is not a string"<<std::endl;
      return false;
    }

    const char* value = json_object_get_string(ha_version_obj);
    if (value == nullptr) {
      std::cerr<<"ParseAuthOk Error getting ha_version value"<<std::endl;
      return false;
    }

    const std::string ha_version = value;
    if (ha_version.empty()) {
      std::cerr<<"ParseAuthOk Invalid ha_version \""<<ha_version<<"\""<<std::endl;
      return false;
    }
  }

  return true;
}

}

namespace homeassistant {

// Traffic Flow
// https://developers.home-assistant.io/docs/api/websocket/
//
// 1. Client connects.
// 2. Authentication phase starts.
//   a. Server sends auth_required message.
//   b. Client sends auth message.
//   c. If auth message correct: go to 3.
//   d. Server sends auth_invalid. Go to 6.
// 3. Server sends auth_ok message
// 4. Authentication phase ends.
// 5. Command phase starts.
//   a. Client can send commands.
//   b. Server can send results of previous commands.
// 6. Client or server disconnects session.

bool Authenticate(curl::cWebSocket& ws, const application::cSettings& settings)
{
  char buffer[2 * 1024] = {0};

  util::msleep(20);

  // 2. Authentication phase starts.
  //   a. Server sends auth_required message.
  {
    const ssize_t nbytes_read = ws.Receive(buffer, sizeof(buffer));
    if (nbytes_read <= 0) {
      std::cerr<<"Receive auth_required returned "<<nbytes_read<<", returning false"<<std::endl;
      return false;
    }

    std::cout<<"Receive auth_required received data: \""<<std::string(buffer, nbytes_read)<<"\""<<std::endl;

    // Parse the message
    if (!ParseAuthRequired(std::string(buffer, nbytes_read))) {
      std::cerr<<"Receive auth_required was not valid, returning false"<<std::endl;
      return false;
    }
  }

  util::msleep(50);

  //   b. Client sends auth message.
  // {
  //  "type": "auth",
  //  "access_token": "ABCDEFGH"
  // }
  {
    const std::string auth_message =
"{\n"
"  \"type\": \"auth\",\n"
"  \"access_token\": \"" + settings.GetAPIToken() + "\"\n"
"}\n";
    const ssize_t nbytes_written = ws.Send(auth_message);
    if (nbytes_written != auth_message.length()) {
      std::cerr<<"Send auth_message returned "<<nbytes_written<<", returning false"<<std::endl;
      return false;
    }
  }

  util::msleep(50);

  // 3. Server sends auth_ok message
  // {
  //   "type": "auth_ok",
  //   "ha_version": "2021.5.3"
  // }
  {
    const ssize_t nbytes_read = ws.Receive(buffer, sizeof(buffer));
    if (nbytes_read <= 0) {
      std::cerr<<"Receive auth_ok returned "<<nbytes_read<<", returning false"<<std::endl;
      return false;
    }

    std::cout<<"Receive auth_ok received data: \""<<std::string(buffer, nbytes_read)<<"\""<<std::endl;

    // Parse the message
    if (!ParseAuthOk(std::string(buffer, nbytes_read))) {
      std::cerr<<"Receive auth_ok was not valid, returning false"<<std::endl;
      return false;
    }
  }

  ws.SendClose();

  return true;
}

std::string CreateBackup(curl::cWebSocket& ws, const application::cSettings& settings)
{
  (void)ws;
  (void)settings;

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
