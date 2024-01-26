// Standard headers
#include <iostream>
#include <fstream>

// Application headers
#include "websocket.h"

// gtest headers
#include <gtest/gtest.h>

TEST(Application, TestDoSomething)
{
  EXPECT_EQ(12345, websocket::do_something());

  //EXPECT_STREQ(TEXT("29B1"), CalculateCRC<spitfire::cCRC16>("123456789").c_str());
}
