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

TEST(sbin, capacity) {
  sbin stream;
  for (int i = 0; i <= 2; i++){
    size_t new_capacity = stream.capacity() + i;
    
    stream.resize(new_capacity);
    EXPECT_EQ(stream.capacity(), new_capacity);
  }

  for (int i = 0; i <= 2; i++){
    size_t new_capacity = stream.capacity() + i;
    
    stream.reserve(new_capacity);
    EXPECT_EQ(stream.capacity(), new_capacity);
  }

  for (int i = 2; i >= 0; i--) {
    stream.resize(4);
    size_t old_capacity = stream.capacity();
    
    stream.resize(stream.size() - i);
    EXPECT_EQ(stream.capacity(), old_capacity);
  }
}

TEST(sbin, clear_empty) {
  sbin stream;
  EXPECT_TRUE(stream.empty());

  for (uint8_t i = 0; i <= 2; i++) {
    stream.resize(i);
    if (i == 0)
      EXPECT_TRUE(stream.empty());
    else
      EXPECT_FALSE(stream.empty());
  }
  stream.clear();
  EXPECT_TRUE(stream.empty());
}
