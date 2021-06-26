#include "nbt-blacksmith/primitive.hpp"
namespace blacksmith {
  template<>
  uint8_t PrimitiveTag<uint8_t>::type() const {
    return nbt_type::BYTE;
  }
  
  template<>
  uint8_t PrimitiveTag<int16_t>::type() const {
    return nbt_type::SHORT;
  }
  
  template<>
  uint8_t PrimitiveTag<int32_t>::type() const {
    return nbt_type::INT;
  }
  
  template<>
  uint8_t PrimitiveTag<int64_t>::type() const {
    return nbt_type::LONG;
  }
  
  template<>
  uint8_t PrimitiveTag<float>::type() const {
    return nbt_type::FLOAT;
  }
  
  template<>
  uint8_t PrimitiveTag<double>::type() const {
    return nbt_type::DOUBLE;
  }
  
  template<>
  uint8_t PrimitiveTag<string>::type() const {
    return nbt_type::STRING;
  }
}
