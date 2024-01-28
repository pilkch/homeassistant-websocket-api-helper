// Application headers
#include "settings.h"

// gtest headers
#include <gtest/gtest.h>

TEST(Application, TestSettings)
{
  application::cSettings settings;
  ASSERT_TRUE(settings.LoadFromFile("test/data/configuration.json"));

  EXPECT_STREQ("ABCDEFGHIJKLMNOPQ", settings.GetAPIToken().c_str());
  EXPECT_STREQ("./myserver.network.home.crt", settings.GetSelfSignedCertificate().c_str());
}
