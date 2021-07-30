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

  for (uint8_t value = 0; value <= 2; value++)
    for (uint8_t size = 0; size <= 2; size++) {
      stream.clear();
      stream.resize(size, value);

      for (const uint8_t &element : stream)
	EXPECT_EQ(element, value);

      stream.clear();
      stream.resize(size);

      for (const uint8_t &element : stream)
	EXPECT_EQ(element, 0);
    }

  for (uint8_t old_size = 0; old_size <= 2; old_size++)
    for (uint8_t new_size = 0; new_size <= 2; new_size++)
      for (uint8_t position = 0; position < old_size; position++) {
	stream.clear();
	stream.resize(old_size);
      
	stream.seek(stream.begin() + position);
	stream.resize(new_size);

	if (new_size < old_size)
	  EXPECT_TRUE(stream.cur() == stream.begin());
	else
	  EXPECT_EQ(stream.cur(), stream.begin() + position);
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

  stream.resize(3);
  
  for (uint8_t old_capacity = 0; old_capacity <= 2; old_capacity++)
    for (uint8_t new_capacity = 0; new_capacity <= 2; new_capacity++)
      for (uint8_t position = 0; position < 3; position++) {
	stream.reserve(old_capacity);
      
	stream.seek(stream.begin() + position);
	stream.reserve(new_capacity);

	EXPECT_EQ(stream.cur(), stream.begin() + position);
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
  EXPECT_EQ(stream.begin(), stream.cur());
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
  const uint8_t RESERVED = 10,
    REAL_DATA = 0xbb,
    OVERFLOW_PROTECTION = 0xff;

  ASSERT_NE(REAL_DATA, OVERFLOW_PROTECTION);
  
  stream.resize(RESERVED, OVERFLOW_PROTECTION);
  
  for (int8_t size = 2; size >= 0; size--) {
    stream.clear();
    stream.resize(size, REAL_DATA);
    
    for (int8_t i = 0; i < RESERVED; i++) {
      uint8_t result = stream.get();
      
      if (i < size) {
	EXPECT_EQ(result, REAL_DATA);

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

TEST(sbin, parsing) {
  sbin stream;

  uint8_t should_be_zero = 0;
  stream >> should_be_zero;
  EXPECT_EQ(should_be_zero, 0);
  EXPECT_EQ(stream.cur(), stream.begin());

  stream << (uint8_t) 0xff;
  EXPECT_EQ(stream.cur(), stream.begin());

  ASSERT_EQ(stream.size(), 1);
  ASSERT_EQ(stream.peek(), 0xff);
  
  uint16_t two_bytes = 0;
  stream >> two_bytes;
  EXPECT_EQ(stream.cur(), stream.end());
  EXPECT_EQ(two_bytes, 0xff00);
  
}

TEST(sbin, eq_operator) {
  sbin left, right;
  EXPECT_TRUE(left == right);

  for (int size_left = 0; size_left <= 2; size_left++) {
    left.clear();
    left.resize(size_left, 0xff);
    
    for (int size_right = 0; size_right <= 2; size_right++) {
      right.clear();
      right.resize(size_right, 0xff);
      if (size_left == size_right)
	EXPECT_TRUE(left == right);
      else
	EXPECT_FALSE(left == right);
    }
    right.clear();
    right.resize(size_left, 0x00);
    if (size_left != 0)
      EXPECT_FALSE(left == right);
    else
      EXPECT_TRUE(left == right);
  }
}

TEST(sbin, assign_operator) {
  sbin left, right;
  
  for (int8_t size = 2; size >= 0; size--) {
    for (size_t position = 0; position < size; position++) {
      right.clear();
      right.resize(size, 0xbb);
	right.seek(right.begin() + position);
      
	left = right;
	EXPECT_TRUE(left == right);
    
	EXPECT_EQ(distance(left.begin(), left.cur()), position);
	EXPECT_EQ(distance(right.begin(), right.cur()), position);
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
  }

  TEST(sbin, constructor) {
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
