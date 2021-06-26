#include <gtest/gtest.h>
#include <cstdint>
#include <nbt-blacksmith/nbt.hpp>

using namespace std;
using namespace blacksmith;

TEST(sbin, size) {
  sbin stream;

  EXPECT_TRUE(stream.eos());
  EXPECT_EQ(stream.size(), 0);
  
  for (int i = 2; i >= 0; i--) {
    stream.resize(i);

    if (i != 0)
      EXPECT_FALSE(stream.eos());
    else
      EXPECT_TRUE(stream.eos());
    EXPECT_EQ(stream.size(), i);
    EXPECT_TRUE(stream.cur() == stream.begin());
  }
}

