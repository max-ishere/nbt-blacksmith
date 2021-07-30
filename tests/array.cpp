#include <cstdint>
#include <string>
#include <vector>
#include <gtest/gtest.h>

#include <nbt-blacksmith/sbin.hpp>
#include <nbt-blacksmith/array.hpp>

using namespace std;
using namespace blacksmith;

TEST(array, serialise) {  
  for (uint8_t size = 0; size <= 2; size++) {
    sbin stream;
    sbin expected;

    expected << nbt_type::INT_ARRAY
	     << "abc"
	     << (int32_t) size;

    vector<int32_t> payload;
    for (uint8_t i = 0; i < size; i++) {
      payload.push_back(i + 4);
      expected << (int32_t) i + 4;  
    }
    EXPECT_EQ(payload.size(), size);
    
    ArrayTag<int32_t> tag("abc", payload);
    stream << tag;
    
    EXPECT_TRUE(stream == expected);
  }
}

TEST(array, parse) {
  for (uint8_t size = 0; size <= 2; size++) {
    sbin source;
    source << nbt_type::INT_ARRAY
	   << "abc"
	   << (int32_t) size;

    vector<int32_t> payload;
    for (uint8_t i = 0; i < size; i++) {
	source << (int32_t) i + 4;
	payload.push_back(i + 4);
    }
    
    ArrayTag<int32_t> tag;
    source >> tag;

    EXPECT_EQ(tag.name, "abc");
    EXPECT_EQ(tag.payload, payload);
  }
}
