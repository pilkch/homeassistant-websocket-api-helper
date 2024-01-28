#pragma once

#include <string>

namespace application {

class cSettings {
public:
  bool LoadFromFile(const std::string& file_path);

  bool IsValid() const;
  void Clear();

  const std::string GetAPIToken() const { return api_token; }
  const std::string GetSelfSignedCertificate() const { return self_signed_certificate; }

private:
  std::string api_token;
  std::string self_signed_certificate;
};

}
