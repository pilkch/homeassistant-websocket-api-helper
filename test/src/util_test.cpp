// Application headers
#include "util.h"

// gtest headers
#include <gtest/gtest.h>

TEST(Application, TestUtilGetDateYYYYMMDD)
{
  const std::string result = util::GetDateYYYYMMDD();

  ASSERT_EQ(8, result.length());
  for (auto& c : result) {
    EXPECT_TRUE(std::isdigit(static_cast<unsigned char>(c)));
  }
}
