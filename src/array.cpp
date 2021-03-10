#include "../include/array.hpp"
namespace blacksmith {
  template<>
  uint8_t ArrayTag<uint8_t>::type() const {
    return nbt_type::BYTE_ARRAY;
  }
  
  template<>
  uint8_t ArrayTag<int32_t>::type() const {
    return nbt_type::INT_ARRAY;
  }
  
  template<>
  uint8_t ArrayTag<int64_t>::type() const {
    return nbt_type::LONG_ARRAY;
  }
}
