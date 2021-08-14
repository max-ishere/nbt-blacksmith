#include <gtest/gtest.h>
#include <cstdint>
#include <nbt-blacksmith/nbt.hpp>
#include "test.hpp"
#include <nbt-blacksmith/ios-bin.hpp>
namespace blacksmith::test {
  using namespace std;
  
  template <class T>
  class SbinOperatorsTest : public ::testing::TestWithParam<const int> { };
  
  TYPED_TEST_SUITE(SbinOperatorsTest,
		   NumericTypes,
		   NBTtypenames);
    
  TEST(sbin, string_serialising) {
    for (int16_t length = 0; length <=2; length++) {
      sbin source;
    
      string expected;
      source << length;

      for (int8_t i = 0; i < length; i++) {
	source << (char) ('a' + i);
	expected += (char) ('a' + i);
      }
    
      string str_result;
      source >> str_result;
      EXPECT_EQ(expected, str_result);
    }
  }

  TEST(sbin, string_parsing) {
    for (int16_t length = 0; length <=2; length++) {
      sbin expected, result;
      string str;

      expected << length;
      for (int8_t i = 0; i < length; i++) {
	expected << (char) ('a' + i);
	str += (char) ('a' + i);
      }
    
      result << str;    
      EXPECT_TRUE(expected == result);

      // explicitly testing the quoted string literals
      // which was the only point of
      // sbin& operator<<(sbin&, const char*);
      result.clear();
      switch (length) {
      case 0: 
	result << "";
	break;
      case 1: 
	result << "a";
	break;
      case 2: 
	result << "ab";
	break;
      default: FAIL();
      }

      EXPECT_TRUE(expected == result);
    }
  }

  TYPED_TEST(SbinOperatorsTest, Numeric) {
    for (auto repeats = 0u; repeats <= 2u; repeats++) {
      sbin stream;

      for (auto i = 0u; i < repeats; i++) {
	stream << (TypeParam) (i + 4);
	EXPECT_EQ(stream.size(), sizeof(TypeParam) * (i + 1));
      }
      EXPECT_EQ(stream.size(),
		sizeof(TypeParam) * repeats);

      for (auto i = 0u; i < repeats; i++) {
	TypeParam result = 0;
	stream >> result;

	EXPECT_EQ(result, i + 4);
      }
    }
  }
}
