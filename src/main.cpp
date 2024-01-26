#include <iostream>

#include <curl/curl.h>

int main()
{
  curl_global_init(CURL_GLOBAL_ALL);

  curl_global_cleanup();

  return EXIT_SUCCESS;
}
