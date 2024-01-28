// Standard headers
#include <iostream>
#include <fstream>

// Application headers
#include "util.h"
#include "web_socket.h"

// gtest headers
#include <gtest/gtest.h>

// NOTE: We rely on external web servers for these tests which isn't great, but it is easier than spinning up our own web server

// Even with some hacks I cannot get this server to respond reliably, so it is commented out for the moment
/*TEST(Application, TestEchoPostmanEchoCom)
{
  // This web server is meant to echo whatever we send to it, but it seems to just reply with something like "Request served by 7811941c69e658"
  const std::string URL = "wss://ws.postman-echo.com/raw";

  curl::cWebSocket ws;
  ASSERT_TRUE(ws.open(URL, std::nullopt));

  // Make sure that the web server is ready
  util::msleep(100);

  char buffer[256] = {0};

  // I'd like to call this a few times, but it doesn't seem to respond after the first response?
  for (size_t iter = 0; iter < 10; iter++) {
    std::cout<<"iter: "<<iter<<std::endl;
    const ssize_t nbytes_written = ws.Send("test message " + std::to_string(iter) + "\n");
    EXPECT_EQ(15, nbytes_written);

    util::msleep(100);

    ssize_t nbytes_read = ws.Receive(buffer, sizeof(buffer));

    // HACK: This server seems to be either buggy, set up weirdly, or severly under resourced? It seems to take an extra read at the start of the communications
    if (nbytes_read == 0) {
      // A read failed, so sleep and try again
      util::msleep(100);
      nbytes_read = ws.Receive(buffer, sizeof(buffer));
    }

    EXPECT_EQ(15, nbytes_read);

    // Match the start of something like "Request served by 7811941c69e658"
    EXPECT_STREQ(std::string("test message " + std::to_string(iter) + "\n").c_str(), std::string(buffer, nbytes_read).c_str());

    util::msleep(50);
	}

  ws.SendClose();
}*/

TEST(Application, TestEchoWebSocketOrg)
{
  // This web server is meant to echo whatever we send to it, but it seems to just reply with something like "Request served by 7811941c69e658"
  const std::string URL = "wss://echo.websocket.org/";

  curl::cWebSocket ws;
  ASSERT_TRUE(ws.Open(URL, std::nullopt));

  // Make sure that the web server is ready
  util::msleep(50);

  char buffer[256] = {0};

  // I'd like to call this a few times, but it doesn't seem to respond after the first response?
  for (size_t iter = 0; iter < 1; iter++) {
    std::cout<<"iter: "<<iter<<std::endl;
    const ssize_t nbytes_written = ws.Send("test message " + std::to_string(iter) + "\n");
    EXPECT_EQ(15, nbytes_written);

    util::msleep(50);

    const ssize_t nbytes_read = ws.Receive(buffer, sizeof(buffer));
    EXPECT_EQ(32, nbytes_read);

    // Match the start of something like "Request served by 7811941c69e658"
    EXPECT_STREQ("Request served by ", std::string(buffer, std::min<size_t>(18, nbytes_read)).c_str());

    util::msleep(50);
	}

  ws.SendClose();
}
