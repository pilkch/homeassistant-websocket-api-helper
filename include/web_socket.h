#pragma once

#include <curl/curl.h>

namespace curl {

class web_socket {
public:
  web_socket();
  ~web_socket();

  bool open(const std::string_view url);
  void close();

private:
  CURL* curl;
};

}
