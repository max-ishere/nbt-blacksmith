#pragma once
#include <typeinfo>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "sbin.hpp"

namespace blacksmith {
  using std::string;
  using std::vector;
  using std::shared_ptr;
  using std::make_shared;
  using std::function;
  using info = const std::type_info&;

  enum nbt_type : uint8_t
    {
     END = 0,
      
     BYTE = 1,
     SHORT = 2,
     INT = 3,
     LONG = 4,
     FLOAT = 5,
     DOUBLE = 6,
      
     BYTE_ARRAY = 7,
     STRING = 8,
     LIST = 9,
     COMPOUND = 10,
     INT_ARRAY = 11,
     LONG_ARRAY = 12
    };
  enum nbt_kind : uint8_t
    {
     PRIMITIVE_KIND = 0,     
     ARRAY_KIND = 1,     
     LIST_KIND = 9,
     COMPOUND_KIND = 10
    };
  
  class Tag {
  public:
    string name = "";

    Tag() = default;
    
    Tag(string name) : name(name) { }
    ~Tag() noexcept = default;

    Tag(const Tag &other) = default;
    Tag(Tag &&other) noexcept = default;
    
    Tag& operator=(const Tag &other) = default;
    Tag& operator=(Tag &&other) noexcept = default;

    virtual uint8_t kind() const { return 0; }
    virtual uint8_t type() const { return 0; }
  };
  
  template<class T>
  struct NamelessTag {
    T& tag;
    NamelessTag(T& tag) : tag(tag) { }
  };
  
  template<class T>
  class PrimitiveTag : public Tag {
  public:
    T payload = { };

    PrimitiveTag() = default;
    PrimitiveTag(string name, T payload)
      : Tag(name), payload(payload) { }
    PrimitiveTag(T payload)
      : payload(payload) { }
    PrimitiveTag(const PrimitiveTag &other) = default;
    PrimitiveTag(PrimitiveTag &&other) noexcept = default;
    ~PrimitiveTag() noexcept = default;

    PrimitiveTag& operator=(const PrimitiveTag &other) = default;
    PrimitiveTag& operator=(PrimitiveTag &&other) noexcept = default;

    virtual uint8_t kind() const final override {
      return nbt_kind::PRIMITIVE_KIND;
    }
    
    virtual uint8_t type() const override;
  };

  template<class T>
  class ArrayTag : public Tag {
  public:
    vector<T> payload = { };

    ArrayTag() = default;
    ArrayTag(string name, vector<T> payload)
      : Tag(name), payload(payload) { }
    ArrayTag(vector<T> payload)
      : payload(payload) { }
    ArrayTag(const ArrayTag &other) = default;
    ArrayTag(ArrayTag &&other) noexcept = default;
    ~ArrayTag() noexcept = default;

    ArrayTag& operator=(const ArrayTag &other) = default;
    ArrayTag& operator=(ArrayTag &&other) noexcept = default;

    int32_t size() const { return payload.size(); }
    
    virtual uint8_t kind() const final override {
      return nbt_kind::ARRAY_KIND;
    }
    
    virtual uint8_t type() const override;
  };

  class TypelessList : public Tag {
    int32_t lenght = 0;
  public:
    TypelessList() = default;
    TypelessList(string name)
      : Tag(name) { }
    TypelessList(const TypelessList &other) = default;
    TypelessList(TypelessList &&other) noexcept = default;
    ~TypelessList() noexcept = default;

    TypelessList& operator=(const TypelessList &other) = default;
    TypelessList& operator=(TypelessList &&other) noexcept = default;
    
    virtual uint8_t kind() const final override {
      return nbt_type::LIST;
    }

    virtual uint8_t type() const override { return 0; }
  };
  
  template<class T>
  class ListTag : public TypelessList {
  public:
    vector<T> payload = { };

    ListTag() = default;
    ListTag(string name, vector<T> payload)
      : TypelessList(name), payload(payload) { }
    ListTag(vector<T> payload)
      : payload(payload) { }
    ListTag(const ListTag &other) = default;
    ListTag(ListTag &&other) noexcept = default;
    ~ListTag() noexcept = default;

    ListTag& operator=(const ListTag &other) = default;
    ListTag& operator=(ListTag &&other) noexcept = default;
    
    virtual uint8_t type() const override;
  };

  template<>
  class ListTag<void> : public TypelessList {
  public:
    int32_t length = 0;

    ListTag() = default;
    ListTag(string name, int32_t length)
      : TypelessList(name), length(length) { }
    ListTag(int32_t length)
      : length(length) { }
    ListTag(const ListTag &other) = default;
    ListTag(ListTag &&other) noexcept = default;
    ~ListTag() noexcept = default;

    ListTag& operator=(const ListTag &other) = default;
    ListTag& operator=(ListTag &&other) noexcept = default;
    
    virtual uint8_t type() const override;
  };
  
  class CompoundTag : public Tag {
  public:
    vector<shared_ptr<Tag>> payload = { };

    CompoundTag() = default;
    CompoundTag(string name, vector<shared_ptr<Tag>> payload)
      : Tag(name), payload(payload) { }
    CompoundTag(vector<shared_ptr<Tag>> payload)
      : payload(payload) { }
    CompoundTag(const CompoundTag &other) = default;
    CompoundTag(CompoundTag &&other) noexcept = default;
    ~CompoundTag() noexcept = default;

    CompoundTag& operator=(const CompoundTag &other) = default;
    CompoundTag& operator=(CompoundTag &&other) noexcept = default;

    virtual uint8_t kind() const final override {
      return nbt_kind::COMPOUND_KIND;
    }

    virtual uint8_t type() const final override {
      return nbt_type::COMPOUND;

    }
    size_t size() const { return payload.size(); }
  };
}
#include "t-nbt.hpp"
