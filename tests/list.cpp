#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <gtest/gtest.h>
#include "test.hpp"

#include <nbt-blacksmith/tag.hpp>
#include <nbt-blacksmith/sbin.hpp>
#include <nbt-blacksmith/list.hpp>
#include <nbt-blacksmith/ios-bin.hpp>

namespace blacksmith::test {
  using std::vector;
  using std::cout;
  
  template<class T>
  class ListTagTest : public ::testing::Test { };

  TYPED_TEST_SUITE(ListTagTest,
		   NumericTypes,
		   NBTtypenames);
  
  TYPED_TEST(ListTagTest, Serialise) {
    for (int32_t size = 0; size <= 2; size++) {
      ListTag<TypeParam> tag("abc");
      
      sbin expected;
      expected << nbt_type::LIST
	       << "abc"
	       << tag.type()
	       << size;
    
      vector<TypeParam> payload;
      for (auto i = 0u; i < size; i++) {
	payload.push_back(i + 4);
	expected << (TypeParam) (i + 4);
      }
      
      tag.payload = payload;
      
      sbin result;
      result << tag;

      EXPECT_EQ(result, expected);
    }
  }

  TYPED_TEST(ListTagTest, Parse) {
    for (int32_t size = 0; size <= 2; size++) {
      sbin source;
      ListTag<TypeParam> tag;
      
      source << nbt_type::LIST
	     << "abc"
	     << tag.type()
	     << size;

      vector<TypeParam> payload;
      for (auto i = 0u; i < size; i++) {
	source << (TypeParam) (i + 4);
	payload.push_back(i + 4);
      }

      source >> tag;

      EXPECT_EQ(tag.name, "abc");
      EXPECT_EQ(tag.payload, payload);
    }
  }

  TYPED_TEST(ListTagTest, NamelessSerialise) {
    for (uint8_t size = 0; size <= 2; size++) {
      ListTag<TypeParam> tag("abc");
      
      sbin expected;
      expected << tag.type()
	       << (int32_t)size;

      vector<TypeParam> payload;
      for (auto i = 0u; i < size; i++) {
	payload.push_back(i + 4);
	expected << (TypeParam) (i + 4);
      }
      
      tag.payload = payload;

      sbin result;
      NamelessTag<ListTag<TypeParam> > nameless_tag(tag);
      result << nameless_tag;

      EXPECT_EQ(result, expected);
    }
  }
  
  TYPED_TEST(ListTagTest, NamelessParse) {
    for (int32_t size = 0; size <= 2; size++) {
      sbin source;
      ListTag<TypeParam> tag;
      
      source << tag.type()
	     << size;

      vector<TypeParam> payload;
      for (auto i = 0u; i < size; i++) {
	source << (TypeParam) (i + 4);
	payload.push_back(i + 4);
      }

      NamelessTag<ListTag<TypeParam>> nameless_tag(tag);
      source >> nameless_tag;

      EXPECT_EQ(tag.name, "");
      EXPECT_EQ(tag.payload, payload);
    }
  }
  
  TYPED_TEST(ListTagTest, NestedSerialise) {
    for (int32_t outter_size = 0; outter_size <= 2; outter_size++) {
      for (int32_t nested_size = 0; nested_size <= 2; nested_size++) {
	for (int32_t inner_size = 0; inner_size <= 2; inner_size++) {
	  ListTag<ListTag<ListTag<TypeParam> > > outter_list("abc");
	  
	  sbin expected;
	  expected << outter_list.kind()
		   << "abc"
		   << outter_list.type()
		   << outter_size;
	
	  for (uint8_t i = 0; i < outter_size; i++) {
	    ListTag<ListTag<TypeParam> > nested_list;
	    
	    expected << nested_list.type()
		     << nested_size;

	    for (uint8_t j = 0; j < nested_size; j++) {
	      ListTag<TypeParam> inner_list;
	      
	      expected << inner_list.type()
		       << inner_size;
	     
	      for (uint8_t k = 0; k < inner_size; k++) {
		expected << (TypeParam) (k + 4);

		inner_list.push_back(k + 4);
	      }
	      nested_list.push_back(inner_list);
	    }
	    outter_list.push_back(nested_list);
	  }

	  sbin result;
	  result << outter_list;
	  
	  EXPECT_EQ(result, expected);
	}
      }
    }
  }

  TYPED_TEST(ListTagTest, NestedParse) {
    for (int32_t outter_size = 0; outter_size <= 2; outter_size++) {
      for (int32_t nested_size = 0; nested_size <= 2; nested_size++) {
	for (int32_t inner_size = 0; inner_size <= 2; inner_size++) {
	  ListTag<ListTag<ListTag<TypeParam> > > tag;
	  sbin source;
	
	  source << tag.kind()
		 << "abc"
		 << tag.type()
		 << outter_size;

	  vector<vector<vector<int16_t> > > outter_list;
	  for (uint8_t i = 0; i < outter_size; i++) {
	    source << nbt_type::LIST
		   << nested_size;

	    vector<vector<int16_t> > nested_list;
	    for (uint8_t j = 0; j < nested_size; j++) {
	      ListTag<TypeParam> t;
	      source << t.type()
		     << inner_size;

	      vector<int16_t> inner_list;
	      for (uint8_t k = 0; k < inner_size; k++) {
		source << (TypeParam) (k + 4);
		inner_list.push_back(k + 4);
	      }
	      nested_list.push_back(inner_list);
	    }
	    outter_list.push_back(nested_list);
	  }

	  source >> tag;

	  // using asserts because we dont want to read garbage from the memory if the list size is different or insufficient
	  ASSERT_EQ(tag.size(), outter_size);
	  ASSERT_EQ(outter_list.size(), outter_size);
	
	  for (int32_t i = 0; i < outter_size; i++) {
	    ASSERT_EQ(tag[i].size(), nested_size);
	    ASSERT_EQ(outter_list[i].size(), nested_size);
	  
	    for (int32_t j = 0; j < nested_size; j++) {
	      ASSERT_EQ(tag[i][j].size(), inner_size);
	      ASSERT_EQ(outter_list[i][j].size(), inner_size);
	      
	      for (int32_t k = 0; k < inner_size; k++) {
		EXPECT_EQ(outter_list[i][j][k], tag[i][j][k]);
	      }
	    }
	  }
	}
      }
    }
  }
}
