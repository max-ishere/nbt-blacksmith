#include <gtest/gtest.h>
#include <cstdint>
#include <nbt-blacksmith/nbt.hpp>

using namespace std;
using namespace blacksmith;

TEST(sbin, size) {
  sbin stream;

  EXPECT_TRUE(stream.eos());
  EXPECT_EQ(stream.size(), 0);
  EXPECT_EQ(stream.cur(), stream.begin());
  
  for (int8_t i = 2; i >= 0; i--) {
    stream.resize(i);

    if (i != 0)
      EXPECT_FALSE(stream.eos());
    else
      EXPECT_TRUE(stream.eos());
    
    EXPECT_EQ(stream.size(), i);
    EXPECT_EQ(stream.cur(), stream.begin());
  }

  stream.clear();
  EXPECT_EQ(stream.begin(), stream.cur());
  for (uint8_t value = 0; value <= 2; value++)
    for (uint8_t i = 0; i <= 2; i++) {
      stream.resize(i, value);

      for (uint8_t &element : stream)
	EXPECT_EQ(element, value);
    }
}

TEST(sbin, capacity) {
  sbin stream;
  
  EXPECT_TRUE(stream.eos());
  EXPECT_EQ(stream.cur(), stream.begin());

  for (int8_t i = 0; i <= 2; i++){
    size_t new_capacity = stream.capacity() + i;
    size_t old_size = stream.size();
    
    stream.reserve(new_capacity);
    EXPECT_EQ(stream.capacity(), new_capacity);
    EXPECT_EQ(stream.size(), old_size);
    EXPECT_EQ(stream.cur(), stream.begin());
    EXPECT_TRUE(stream.eos());
  }
  
  for (int8_t i = 0; i <= 2; i++){
    size_t new_capacity = stream.capacity() + i;
    
    stream.resize(new_capacity);
    EXPECT_GE(stream.size(), new_capacity);
    EXPECT_EQ(stream.cur(), stream.begin());

    if (stream.size() == 0)
      EXPECT_TRUE(stream.eos());
    else
      EXPECT_FALSE(stream.eos());
  }
  
  for (int8_t i = 2; i >= 0; i--) {
    stream.resize(4);
    size_t old_capacity = stream.capacity();
    
    stream.resize((stream.size() - i > 0
		   ? stream.size() - i
		   : 0));
    EXPECT_EQ(stream.capacity(), old_capacity);
    EXPECT_EQ(stream.cur(), stream.begin());

    if (stream.size() == 0)
      EXPECT_TRUE(stream.eos());
    else
      EXPECT_FALSE(stream.eos());
  }
}

TEST(sbin, clear_empty) {
  sbin stream;
  EXPECT_TRUE(stream.empty());

  for (uint8_t i = 0; i <= 2; i++) {
    stream.resize(i);
    if (i == 0) {
      EXPECT_TRUE(stream.empty());
      EXPECT_TRUE(stream.eos());
      EXPECT_EQ(stream.size(), 0);
    }
    else {
      EXPECT_FALSE(stream.empty());
      EXPECT_FALSE(stream.eos());
      EXPECT_NE(stream.size(), 0);
    }
  }
  stream.clear();
  EXPECT_TRUE(stream.empty());
  EXPECT_TRUE(stream.eos());
}

TEST(sbin, seek) {
  sbin stream;

  for (uint8_t size = 0; size <= 2; size++) {
    stream.resize(size);
    
    for (int8_t i = 0; i <= 3; i++) {
      stream.seek(stream.begin());
      vector<uint8_t>::iterator expected
	= ((size - i >= 0)
	   ? stream.begin() + i
	   : stream.cur());
      
      stream.seek(stream.begin() + i);
      EXPECT_EQ(stream.cur(), expected);
      if(expected == stream.end())
	EXPECT_TRUE(stream.eos());
      else
	EXPECT_FALSE(stream.eos());
    }
  }
}

TEST(sbin, get) {
  sbin stream;
  const uint8_t RESERVED = 10;
  stream.resize(RESERVED, 0xff);
  
  for (int8_t size = 2; size >= 0; size--) {
    stream.clear();
    stream.resize(size, 0xbb);
    
    for (int8_t i = 0; i < RESERVED; i++) {
      uint8_t result = stream.get();
      
      if (i < size) {
	EXPECT_EQ(result, 0xbb);

	if (i == size - 1)
	  EXPECT_TRUE(stream.eos());
	else
	  EXPECT_FALSE(stream.eos()) << (int)result;
      }
      else {
	EXPECT_EQ(result, 0);
	EXPECT_TRUE(stream.eos());
      }
    }
  }
}

TEST(sbin, peek) {
  sbin stream;
  const uint8_t RESERVED = 10;
  stream.resize(RESERVED, 0xff);
  
  for (int8_t size = 2; size >= 0; size--) {
    stream.clear();
    stream.resize(size, 0xbb);
    
    for (int8_t i = 0; i < RESERVED; i++) {
      uint8_t result = stream.peek();
      
      if (i < size) {
	EXPECT_EQ(result, 0xbb);
	EXPECT_FALSE(stream.eos());
      }
      else {
	EXPECT_EQ(result, 0);
	EXPECT_TRUE(stream.eos());
      }
      stream.get();
    }
  }
}

TEST(sbin, eq_operator) {
  sbin left, right;
  EXPECT_TRUE(left == right);

  for (int i = 0; i <= 2; i++) {
    left.clear();
    left.resize(i, 0xff);
    
    for (int j = 0; j <= 2; j++) {
      right.clear();
      right.resize(j, 0xff);
      if (i == j)
	EXPECT_TRUE(left == right);
      else
	EXPECT_FALSE(left == right);
    }
    right.clear();
    right.resize(i, 0x00);
    if (i != 0)
      EXPECT_FALSE(left == right);
    else
      EXPECT_TRUE(left == right);
  }
}

TEST(sbin, assign_operator) {
  sbin left, right;
  
  for (int8_t size = 2; size >= 0; size--) {
    right.clear();
    right.resize(size, 0xbb);
    left = right;
    EXPECT_TRUE(left == right);
    
    EXPECT_EQ(left.begin(), left.cur());
    EXPECT_EQ(right.begin(), right.cur());
    if (size != 0) {
      EXPECT_FALSE(left.eos());
      EXPECT_FALSE(right.eos());
    }
    else {
      EXPECT_TRUE(left.eos());
      EXPECT_TRUE(right.eos());
    }
  }
}

TEST(sbin, iterator_validity) {
  {
    sbin stream;
    EXPECT_EQ(stream.begin(), stream.cur());
  }

  for (uint8_t size = 0; size <= 2; size++) {
    vector<uint8_t> data;
    data.resize(size, 0xff);
    sbin stream(data);
    
    EXPECT_EQ(stream.begin(), stream.cur());
  }
}
