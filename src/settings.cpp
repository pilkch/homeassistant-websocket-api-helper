#include <climits>
#include <cstring>

#include <limits>
#include <iostream>
#include <filesystem>

#include <pwd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <json-c/json.h>

#include "json.h"
#include "settings.h"
#include "util.h"

namespace application {

bool cSettings::LoadFromFile(const std::string& sFilePath)
{
  Clear();

  const size_t nMaxFileSizeBytes = 20 * 1024;
  std::string contents;
  if (!util::ReadFileIntoString(sFilePath, nMaxFileSizeBytes, contents)) {
    std::cerr<<"File \""<<sFilePath<<"\" not found"<<std::endl;
    return false;
  }

  util::cJSONDocument document(json_tokener_parse(contents.c_str()));
  if (!document.IsValid()) {
    std::cerr<<"Invalid JSON config \""<<sFilePath<<"\""<<std::endl;
    return false;
  }

  // Parse the JSON tree

  // Parse "settings"
  json_object_object_foreach(document.Get(), settings_key, settings_val) {
    enum json_type type_settings = json_object_get_type(settings_val);
    if ((type_settings != json_type_object) || (strcmp(settings_key, "settings") != 0)) {
      std::cerr<<"settings object not found"<<std::endl;
      return false;
    }

    // Parse api_token
    {
      struct json_object* api_token_obj = json_object_object_get(settings_val, "api_token");
      if (api_token_obj == nullptr) {
        std::cerr<<"api_token not found"<<std::endl;
        return false;
      }

      enum json_type api_token_type = json_object_get_type(api_token_obj);
      if (api_token_type != json_type_string) {
        std::cerr<<"api_token is not a string"<<std::endl;
        return false;
      }

      const char* value = json_object_get_string(api_token_obj);
      if (value == nullptr) {
        std::cerr<<"Error getting api_token value"<<std::endl;
        return false;
      }

      api_token = value;
      if (api_token.empty()) {
        std::cerr<<"Invalid api_token"<<std::endl;
        return false;
      }
    }

    // Parse self_signed_certificate (NOTE: This is optional so we only parse it if it is present)
    {
      struct json_object* self_signed_certificate_obj = json_object_object_get(settings_val, "self_signed_certificate");
      if (self_signed_certificate_obj != nullptr) {
        enum json_type self_signed_certificate_type = json_object_get_type(self_signed_certificate_obj);
        if (self_signed_certificate_type != json_type_string) {
          std::cerr<<"self_signed_certificate is not a string"<<std::endl;
          return false;
        }

        const char* value = json_object_get_string(self_signed_certificate_obj);
        if (value == nullptr) {
          std::cerr<<"self_signed_certificate is not a string"<<std::endl;
          return false;
        }

        self_signed_certificate = value;
      }
    }

    // Parse host_name
    {
      struct json_object* host_name_obj = json_object_object_get(settings_val, "host_name");
      if (host_name_obj == nullptr) {
        std::cerr<<"host_name not found"<<std::endl;
        return false;
      }

      enum json_type host_name_type = json_object_get_type(host_name_obj);
      if (host_name_type != json_type_string) {
        std::cerr<<"host_name is not a string"<<std::endl;
        return false;
      }

      const char* value = json_object_get_string(host_name_obj);
      if (value == nullptr) {
        std::cerr<<"host_name is not valid"<<std::endl;
        return false;
      }

      host_name = value;
    }

    // Parse port
    {
      struct json_object* port_obj = json_object_object_get(settings_val, "port");
      if (port_obj == nullptr) {
        std::cerr<<"port not found"<<std::endl;
        return false;
      }

      enum json_type port_type = json_object_get_type(port_obj);
      if (port_type != json_type_int) {
        std::cerr<<"port is not an int"<<std::endl;
        return false;
      }

      const int value = json_object_get_int(port_obj);
      if ((value <= 0) || (value > USHRT_MAX)) {
        std::cerr<<"port is not valid"<<std::endl;
        return false;
      }

      port = value;
    }
  }

  return IsValid();
}

bool cSettings::IsValid() const
{
  // Check the API token, hostname and port
  // The self signed certificate is optional
  return (!api_token.empty() && !host_name.empty() && (port != 0));
}

void cSettings::Clear()
{
  api_token.clear();
  self_signed_certificate.clear();
  host_name.clear();
  port = 0;
}

}
