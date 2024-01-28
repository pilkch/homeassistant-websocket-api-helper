#include <curl/curl.h>

#include "curl_helper.h"

namespace curl {

cCurlHelper::cCurlHelper()
{
  curl_global_init(CURL_GLOBAL_ALL);
}

cCurlHelper::~cCurlHelper()
{
  curl_global_cleanup();
}

}
