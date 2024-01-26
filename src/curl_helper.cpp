#include <curl/curl.h>

#include "curl_helper.h"

namespace curl {

curl_helper::curl_helper()
{
  curl_global_init(CURL_GLOBAL_ALL);
}

curl_helper::~curl_helper()
{
  curl_global_cleanup();
}

}
