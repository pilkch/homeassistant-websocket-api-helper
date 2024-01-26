// gtest headers
#include <gtest/gtest.h>

#include "curl_helper.h"

int main(int argc, char** argv)
{
  curl::curl_helper helper;

  ::testing::InitGoogleTest(&argc, argv);

  const int result = RUN_ALL_TESTS();

  return result;
}
