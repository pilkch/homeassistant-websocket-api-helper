#pragma once

#include <cinttypes>

#include <optional>

#include <curl/curl.h>

namespace curl {

class cWebSocket {
public:
  cWebSocket();
  ~cWebSocket();

  // NOTE: Open can optionally take a self signed certificate stored in a local file something like "./myserver.network.home.crt"
  bool Open(std::string_view url, const std::optional<std::string>& self_signed_certificate_path);
  void Close();

  ssize_t Send(std::string_view message);
  bool SendClose(); // Call this before closing the connection
  ssize_t Receive(char* out_data, size_t out_data_length);
  ssize_t Receive(char* out_data, size_t out_data_length, uint32_t timeout_ms);

private:
  CURL* curl;
};

}
