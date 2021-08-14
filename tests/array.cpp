#include <cstdint>
#include <string>
#include <vector>
#include <gtest/gtest.h>
#include "test.hpp"
#include <nbt-blacksmith/ios-bin.hpp>

#include <nbt-blacksmith/sbin.hpp>
#include <nbt-blacksmith/array.hpp>

namespace blacksmith::test {
  using std::vector;
  
  template<class T>
  class ArrayTagTest : public ::testing::Test { };
  
  TYPED_TEST_SUITE(ArrayTagTest,
		   ArrayTagTypes,
		   NBTtypenames);
  
  TYPED_TEST(ArrayTagTest, Serialise) {  
    for (int32_t size = 0; size <= 2; size++) {
      sbin result,
	expected;

      ArrayTag<TypeParam> tag("abc");
      
      expected << tag.type()
	       << "abc"
	       << size;

      vector<TypeParam> payload;
      for (auto i = 0; i < size; i++) {
	payload.push_back(i + 4);
	expected << (TypeParam) (i + 4);  
      }
      tag.payload = payload;
      
      result << tag;
      EXPECT_EQ(result,  expected);
    }
  }

  TYPED_TEST(ArrayTagTest, Parse) {
    for (int32_t size = 0; size <= 2; size++) {
      sbin source;
      ArrayTag<TypeParam> tag;
      
      source << tag.type()
	     << "abc"
	     << size;

      vector<TypeParam> payload;
      for (TypeParam i = 0; i < size; i++) {
	source << (TypeParam) (i + 4);
	payload.push_back(i + 4);
      }

      ASSERT_EQ(source.cur(), source.begin());
      source >> tag;

      EXPECT_EQ(tag.name, "abc");
      EXPECT_EQ(tag.payload, payload);
    }
  }

  TYPED_TEST(ArrayTagTest, NamelessSerialise) {
    for (int32_t size = 0; size <= 2; size++) {
      sbin result,
	expected;
      ArrayTag<TypeParam> tag;
      NamelessTag<ArrayTag<TypeParam> > nameless_tag(tag);
      
      expected << size;

      vector<TypeParam> payload;
      for (auto i = 0u; i < size; i++) {
	tag.push_back(i + 4);
	expected << (TypeParam) (i + 4);  
      }
      
      result << nameless_tag;

      EXPECT_EQ(expected, result);
    }
  }
  
  TYPED_TEST(ArrayTagTest, NamelessParse) {
    for (int32_t size = 0; size <= 2; size++) {
      sbin source;
      ArrayTag<TypeParam> tag;
      NamelessTag<ArrayTag<TypeParam> > nameless_tag(tag);
    
      source << size;

      vector<TypeParam> payload;
      for (auto i = 0u; i < size; i++) {
	source << (TypeParam) (i + 4);
	payload.push_back(i + 4);
      }
    
      source >> nameless_tag;

      EXPECT_EQ(tag.name, "");
      EXPECT_EQ(tag.payload, payload);
    }
  }
}

