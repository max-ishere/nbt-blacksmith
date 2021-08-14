#pragma once
namespace blacksmith::test {
  class NBTtypenames {
  public:
    template <typename T>
    static std::string GetName(int) {
      if constexpr (std::is_same<T, uint8_t>()) return "byte";
      if constexpr (std::is_same<T, int16_t>()) return "short";
      if constexpr (std::is_same<T, int32_t>()) return "int";
      if constexpr (std::is_same<T, int64_t>()) return "long";
      if constexpr (std::is_same<T, float>()) return "float";
      if constexpr (std::is_same<T, double>()) return "double";
      if constexpr (std::is_same<T, std::string>()) return "string";
      return "INVALID_TYPE";
    }
  };
  
  using PrimiteTagTypes = ::testing::Types<
    uint8_t, int16_t,
    int32_t, int64_t,
    float,   double
    >;

  using ArrayTagTypes = ::testing::Types<
    uint8_t, 
    int32_t,
    int64_t
    >;

  using NumericTypes = ::testing::Types<
    uint8_t, int16_t,
    int32_t, int64_t,
    float,   double
    >;
}
