// Application headers
#include "home_assistant.h"

// gtest headers
#include <gtest/gtest.h>

namespace homeassistant {

// {
//   "type": "auth_required",
//   "ha_version": "2021.5.3"
// }
bool ParseAuthRequired(const std::string& json);

// {
//   "type": "auth_ok",
//   "ha_version": "2021.5.3"
// }
bool ParseAuthOk(const std::string& json);

// {
//   "id":37,
//   "type":"result",
//   "success":true,
//   "result": {
//     "slug":"7d249464",
//     "name":"Core 2023.10.5",
//     "date":"2024-01-29T00:08:06.448735+11:00",
//     "path":"/config/backups/7d249464.tar",
//     "size":5.42
//   }
// }
std::string ParseBackupCreatedResult(const std::string& json, uint32_t expected_id);

}

TEST(Application, TestHomeAssistantParseAuthRequired)
{
  const std::string json_invalid =
"{\n"
"  \"type\": \"incorrect\",\n"
"  \"ha_version\": \"some other value\"\n"
"}\n";
  EXPECT_FALSE(homeassistant::ParseAuthRequired(json_invalid));

  const std::string json_corrupt =
"{\n"
"  \"type\": \"auth_required\",\n";
  EXPECT_FALSE(homeassistant::ParseAuthRequired(json_corrupt));

  const std::string json_valid =
"{\n"
"  \"type\": \"auth_required\",\n"
"  \"ha_version\": \"2021.5.3\"\n"
"}\n";
  EXPECT_TRUE(homeassistant::ParseAuthRequired(json_valid));
}

TEST(Application, TestHomeAssistantParseAuthOk)
{
  const std::string json_invalid =
"{\n"
"  \"type\": \"incorrect\",\n"
"  \"ha_version\": \"some other value\"\n"
"}\n";
  EXPECT_FALSE(homeassistant::ParseAuthOk(json_invalid));

  const std::string json_corrupt =
"{\n"
"  \"type\": \"auth_ok\",\n";
  EXPECT_FALSE(homeassistant::ParseAuthOk(json_corrupt));

  const std::string json_valid =
"{\n"
"  \"type\": \"auth_ok\",\n"
"  \"ha_version\": \"2021.5.3\"\n"
"}\n";
  EXPECT_TRUE(homeassistant::ParseAuthOk(json_valid));
}

TEST(Application, TestHomeAssistantParseBackupCreatedResult)
{
  const std::string json_invalid =
"{\n"
"  \"type\": \"incorrect\",\n"
"  \"ha_version\": \"some other value\"\n"
"}\n";
  EXPECT_STREQ("", homeassistant::ParseBackupCreatedResult(json_invalid, 1234).c_str());

  const std::string json_corrupt =
"{\n"
"  \"id\":37,\n"
"  \"type\":\"result\",\n"
"  \"success\":true,\n"
"  \"result\": {\n";
  EXPECT_STREQ("", homeassistant::ParseBackupCreatedResult(json_corrupt, 37).c_str());

  const std::string json_valid =
"{\n"
"  \"id\":37,\n"
"  \"type\":\"result\",\n"
"  \"success\":true,\n"
"  \"result\": {\n"
"    \"slug\":\"7d249464\",\n"
"    \"name\":\"Core 2023.10.5\",\n"
"    \"date\":\"2024-01-29T00:08:06.448735+11:00\",\n"
"    \"path\":\"/config/backups/7d249464.tar\",\n"
"    \"size\":5.42\n"
"  }\n"
"}\n";

  // Try an invalid id
  const std::string empty_backup_slug = homeassistant::ParseBackupCreatedResult(json_valid, 12);
  EXPECT_STREQ("", empty_backup_slug.c_str());

  // Try a valid id
  const std::string backup_slug = homeassistant::ParseBackupCreatedResult(json_valid, 37);
  EXPECT_STREQ("7d249464", backup_slug.c_str());
}
