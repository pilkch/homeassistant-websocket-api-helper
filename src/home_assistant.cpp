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

// {
//   "id":37,
//   "type":"result",
//   "success":true,
//   "result": {
//     "slug":"7d249464",
//     "name":"Core 2023.10.5",
//     "date":"2024-01-29T00:08:06.448735+11:00",
//     "path":"/config/backups/7d249464.tar",
//     "size":5.42
//   }
// }
std::string ParseBackupCreatedResult(const std::string& json, uint32_t expected_id)
{
  util::cJSONDocument document(json_tokener_parse(json.c_str()));
  if (!document.IsValid()) {
    std::cerr<<"ParseBackupCreatedResult Invalid JSON, returning false"<<std::endl;
    return "";
  }

  // Parse id
  {
    struct json_object* id_obj = json_object_object_get(document.Get(), "id");
    if (id_obj == nullptr) {
      std::cerr<<"ParseBackupCreatedResult id not found"<<std::endl;
      return "";
    }

    // Make sure the id is an integer
    if (json_object_get_type(id_obj) != json_type_int) {
      std::cerr<<"ParseBackupCreatedResult id is not an int"<<std::endl;
      return "";
    }

    const int id = json_object_get_int(id_obj);
    if (id != expected_id) {
      std::cerr<<"ParseBackupCreatedResult Unexpected id "<<id<<", expected "<<expected_id<<std::endl;
      return "";
    }
  }

  // Parse type
  {
    struct json_object* type_obj = json_object_object_get(document.Get(), "type");
    if (type_obj == nullptr) {
      std::cerr<<"ParseBackupCreatedResult type not found"<<std::endl;
      return "";
    }

    // Make sure the type is a string
    if (json_object_get_type(type_obj) != json_type_string) {
      std::cerr<<"ParseBackupCreatedResult type is not a string"<<std::endl;
      return "";
    }

    const char* value = json_object_get_string(type_obj);
    if (value == nullptr) {
      std::cerr<<"ParseBackupCreatedResult Error getting type value"<<std::endl;
      return "";
    }

    const std::string type = value;
    if (type != "result") {
      std::cerr<<"ParseBackupCreatedResult Invalid type \""<<type<<"\""<<std::endl;
      return "";
    }
  }

  // Parse success
  {
    struct json_object* success_obj = json_object_object_get(document.Get(), "success");
    if (success_obj == nullptr) {
      std::cerr<<"ParseBackupCreatedResult success not found"<<std::endl;
      return "";
    }

    // Make sure the success is a boolean
    if (json_object_get_type(success_obj) != json_type_boolean) {
      std::cerr<<"ParseBackupCreatedResult success is not a boolean"<<std::endl;
      return "";
    }

    const bool value = json_object_get_boolean(success_obj);
    if (!value) {
      std::cerr<<"ParseBackupCreatedResult Error success is false"<<std::endl;
      return "";
    }
  }

  // Parse result
  {
    struct json_object* result_obj = json_object_object_get(document.Get(), "result");
    if (result_obj == nullptr) {
      std::cerr<<"ParseBackupCreatedResult result not found"<<std::endl;
      return "";
    }

    enum json_type type_result = json_object_get_type(result_obj);
    if (type_result != json_type_object) {
      std::cerr<<"ParseBackupCreatedResult result object not found"<<std::endl;
      return "";
    }

    // Parse slug
    {
      struct json_object* slug_obj = json_object_object_get(result_obj, "slug");
      if (slug_obj == nullptr) {
        std::cerr<<"ParseBackupCreatedResult slug not found"<<std::endl;
        return "";
      }

      // Make sure the slug is a string
      if (json_object_get_type(slug_obj) != json_type_string) {
        std::cerr<<"ParseBackupCreatedResult slug is not a string"<<std::endl;
        return "";
      }

      const char* value = json_object_get_string(slug_obj);
      if (value == nullptr) {
        std::cerr<<"ParseBackupCreatedResult Error getting slug value"<<std::endl;
        return "";
      }

      // Return the value of the slug
      return std::string(value);
    }
  }

  return "";
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
      std::cerr<<"Authenticate Receive auth_required returned "<<nbytes_read<<", returning false"<<std::endl;
      return false;
    }

    std::cout<<"Authenticate Receive auth_required received data: \""<<std::string(buffer, nbytes_read)<<"\""<<std::endl;

    // Parse the message
    if (!ParseAuthRequired(std::string(buffer, nbytes_read))) {
      std::cerr<<"Authenticate Receive auth_required was not valid, returning false"<<std::endl;
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
      std::cerr<<"Authenticate Send auth_message returned "<<nbytes_written<<", returning false"<<std::endl;
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
      std::cerr<<"Authenticate Receive auth_ok returned "<<nbytes_read<<", returning false"<<std::endl;
      return false;
    }

    std::cout<<"Authenticate Receive auth_ok received data: \""<<std::string(buffer, nbytes_read)<<"\""<<std::endl;

    // Parse the message
    if (!ParseAuthOk(std::string(buffer, nbytes_read))) {
      std::cerr<<"Authenticate Receive auth_ok was not valid, returning false"<<std::endl;
      return false;
    }
  }

  std::cout<<"Authenticate returning true"<<std::endl;
  return true;
}


// What the Home Assistant web interface does in Firefox when we request that a back is generated:
// --> {"type":"backup/generate","id":37}
// <-- {"id":37,"type":"result","success":true,"result":{"slug":"7d249464","name":"Core 2023.10.5","date":"2024-01-29T00:08:06.448735+11:00","path":"/config/backups/7d249464.tar","size":5.42}} // This is basically a callback to say "The download is ready"
//
// Requesting a path to the backup
// --> {"type":"auth/sign_path","path":"/api/backup/download/d8f3f2b4","id":38}
// <-- {"id":37,"type":"result","success":true,"result":{"path":"/api/backup/download/d8f3f2b4?authSig=jksdjioSDJFKLEjkDfklKLDJFKLDF.JKFDLKDSFnsdlksdnlkjsdijISDJFIdsfjlkSDJFKlJSDFIOSJDKFDFlkdfksdfiJFEJIFEWJFSDNkdVHjivVUiCVUpuvrejfijFOJDSIOJSDOIFSDJFODJFDSIFOjdsKDslfnfewleljeNFElnFKLNFLenLKenfleNENklEFKlefnklEFLEKFENKEFLKEFKLFENesfLKEF"}}
//
// We can then do a regular non-websocket API request to download the file with that path
//
std::string CreateBackup(curl::cWebSocket& ws)
{
  std::cout<<"CreateBackup"<<std::endl;

  util::msleep(50);

  char buffer[1000 * 1024] = {0};

  const uint32_t request_id = 1;

  {
    // Request that a backup be created
    // {"type":"backup/generate","id":37}
    const std::string generate_backup_request =
"{\n"
"  \"type\": \"backup/generate\",\n"
"  \"id\": " + std::to_string(request_id) + "\n"
"}\n";
    std::cout<<"Sending \""<<generate_backup_request<<"\""<<std::endl;
    const ssize_t nbytes_written = ws.Send(generate_backup_request);
    if (nbytes_written != generate_backup_request.length()) {
      std::cerr<<"CreateBackup Send generate_backup_request returned "<<nbytes_written<<", returning false"<<std::endl;
      return "";
    }
  }


  std::string backup_slug;

  {
    // Wait for the back has been created response
    // {"id":37,"type":"result","success":true,"result":{"slug":"7d249464","name":"Core 2023.10.5","date":"2024-01-29T00:08:06.448735+11:00","path":"/config/backups/7d249464.tar","size":5.42}}
    std::cout<<"Waiting for the backup to be created"<<std::endl;

    const uint32_t timeout_seconds = 20; // It can take a while to create the backup
    const ssize_t nbytes_read = ws.Receive(buffer, sizeof(buffer), timeout_seconds * 1000);
    if (nbytes_read <= 0) {
      std::cerr<<"CreateBackup Receive returned "<<nbytes_read<<", returning false"<<std::endl;
      return "";
    }

    std::cout<<"CreateBackup Receive received data: \""<<std::string(buffer, nbytes_read)<<"\""<<std::endl;

    // Parse the message
    backup_slug = ParseBackupCreatedResult(std::string(buffer, nbytes_read), request_id);
    if (backup_slug.empty()) {
      std::cerr<<"Receive backup_slug is not valid, returning false"<<std::endl;
      return "";
    }

    std::cout<<"Backup has been created, backup_slug: "<<backup_slug<<std::endl;
  }

  return backup_slug;
}

bool DownloadBackup(curl::cWebSocket& ws, const application::cSettings& settings, const std::string& backup_hash)
{
  (void)ws;
  (void)settings;
  (void)backup_hash;

  return false;
}

}
