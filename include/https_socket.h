#pragma once

#include <optional>

#include <curl/curl.h>

namespace curl {

class cHTTPSSocket {
public:
  cHTTPSSocket();
  ~cHTTPSSocket();

  // NOTE: Open can optionally take a self signed certificate stored in a local file something like "./myserver.network.home.crt"
  bool Open(std::string_view url, const std::optional<std::string>& self_signed_certificate_path, const std::string& api_token);
  void Close();

  bool DownloadToFile(const std::string& output_file);

private:
  CURL* curl;
};

}
