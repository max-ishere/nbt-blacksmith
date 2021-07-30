#include <cstdint>
#include <string>
#include <gtest/gtest.h>

#include <nbt-blacksmith/tag.hpp>
#include <nbt-blacksmith/sbin.hpp>
#include <nbt-blacksmith/primitive.hpp>

using namespace std;
using namespace blacksmith;

TEST(primitive, serialise) {
  sbin stream;
  PrimitiveTag<int16_t> tag("abc", 4);
  
  stream << tag;

  sbin expected;
  expected << nbt_type::SHORT
	   << "abc"
	   << (uint16_t) 4;

  EXPECT_TRUE(stream == expected);
}

TEST(primitive, parse) {
  sbin source;
  PrimitiveTag<int16_t> tag;

  source << nbt_type::SHORT
	 << "abc"
	 << (uint16_t) 4;

  EXPECT_EQ(source.cur(), source.begin());
  source >> tag;

  EXPECT_EQ(tag.name, "abc");
  EXPECT_EQ((int) tag.payload, 4);
}
