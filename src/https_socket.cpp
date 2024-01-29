#include <cstdio>
#include <cstdlib>

#include <iostream>

#include <unistd.h>

#include "https_socket.h"

namespace {

static size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream)
{
  // Write the data to our file
  const size_t written = fwrite(ptr, size, nmemb, (FILE*)stream);
  return written;
}

}


namespace curl {

cHTTPSSocket::cHTTPSSocket() :
  curl(nullptr)
{
}

cHTTPSSocket::~cHTTPSSocket()
{
  Close();
}

bool cHTTPSSocket::Open(std::string_view url, const std::optional<std::string>& self_signed_certificate_path, const std::string& api_token)
{
  Close();

  curl = curl_easy_init();
  if (curl == nullptr) {
    return false;
  }

  curl_easy_setopt(curl, CURLOPT_URL, url.data());
  curl_easy_setopt(curl, CURLOPT_USERAGENT, url.data());
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // Turn on debugging

  curl_easy_setopt(curl, CURLOPT_XOAUTH2_BEARER, api_token.c_str());
  curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BEARER); // Tell curl to use the bearer above

  // Check if we need to provide a self signed public key
  if (self_signed_certificate_path) {
    curl_easy_setopt(curl, CURLOPT_CAINFO, self_signed_certificate_path.value().c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1);
  }

  return true;
}

bool cHTTPSSocket::DownloadToFile(const std::string& output_file)
{
  // Perform the request and write the response to a file
  FILE* file = fopen(output_file.c_str(), "wb");
  if (!file) {
    std::cerr<<"cHTTPSSocket::DownloadToFile Error opening output file \""<<output_file<<"\""<<std::endl;
    return false;
  }

  // Send all data to this function
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

  // Write the page body to this file handle
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

  // Perform the download
  const CURLcode result = curl_easy_perform(curl);

  // Close the file
  fclose(file);

  return (result == CURLE_OK);
}

void cHTTPSSocket::Close()
{
  curl_easy_cleanup(curl);
  curl = nullptr;
}

}
