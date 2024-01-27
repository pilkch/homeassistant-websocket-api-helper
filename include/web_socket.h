#pragma once

#include <curl/curl.h>

namespace curl {

class web_socket {
public:
  web_socket();
  ~web_socket();

  bool open(const std::string_view url);
  void close();

  ssize_t send(std::string_view message);
  bool send_close(); // Call this before closing the connection
  ssize_t receive(char* data, size_t data_length);

private:
  CURL* curl;
};

}
