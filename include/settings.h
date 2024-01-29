#pragma once

#include <cstdint>
#include <string>

namespace application {

class cSettings {
public:
  bool LoadFromFile(const std::string& file_path);

  bool IsValid() const;
  void Clear();

  const std::string& GetAPIToken() const { return api_token; }
  const std::string& GetSelfSignedCertificate() const { return self_signed_certificate; }
  const std::string& GetHostName() const { return host_name; }
  uint16_t GetPort() const { return port; }

private:
  std::string api_token;
  std::string self_signed_certificate;
  std::string host_name;
  uint16_t port;
};

}
