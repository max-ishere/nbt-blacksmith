#include <cstdint>
#include <string>
#include <gtest/gtest.h>

#include <nbt-blacksmith/tag.hpp>
#include <nbt-blacksmith/sbin.hpp>
#include <nbt-blacksmith/primitive.hpp>
#include <nbt-blacksmith/ios-bin.hpp>
#include "test.hpp"

namespace blacksmith::test {
  using std::vector;
  
  template<class T>
  class PrimitiveTagTest : public ::testing::Test { };

  TYPED_TEST_SUITE(PrimitiveTagTest,
		   PrimiteTagTypes,
		   NBTtypenames);

  TYPED_TEST(PrimitiveTagTest, Serialise) {
    PrimitiveTag<TypeParam> tag("abc", 4);
    
    sbin expected;
    expected << tag.type()
	     << "abc"
	     << (TypeParam) 4;
    
    sbin result;
    result << tag;
    
    EXPECT_TRUE(result == expected);
  }

  TYPED_TEST(PrimitiveTagTest, Parse) {
    sbin source;
    PrimitiveTag<TypeParam> tag;

    source << tag.type()
	   << "abc"
	   << (TypeParam) 4;

    ASSERT_EQ(source.cur(), source.begin());
    source >> tag;

    EXPECT_EQ(tag.name, "abc");
    EXPECT_EQ(tag.payload, (TypeParam) 4);
  }

  TYPED_TEST(PrimitiveTagTest, NamelessSerialise) {
    sbin expected;
    expected << (TypeParam)4;

    PrimitiveTag<TypeParam>tag (4);
    NamelessTag<PrimitiveTag<TypeParam> > nameless_tag(tag);
    
    sbin result;
    result << nameless_tag;

    EXPECT_EQ(result, expected);
  }

  TYPED_TEST(PrimitiveTagTest, NamelessParse) {
    sbin source;
    source << (TypeParam)4;

    PrimitiveTag<TypeParam>tag;
    NamelessTag<PrimitiveTag<TypeParam> > nameless_tag(tag);

    source >> nameless_tag;

    EXPECT_EQ(tag.name, "");
    EXPECT_EQ(tag.payload, 4);
  }
}
