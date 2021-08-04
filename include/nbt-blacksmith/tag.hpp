#pragma once
#include <string>
#include <cstdint>
#include "sbin.hpp"

namespace blacksmith {
  using std::string;
  
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

  sbin& operator<<(sbin& stream, const shared_ptr<Tag>);
  sbin& operator>>(sbin& stream, shared_ptr<Tag>&);

  template<class T>
  sbin& operator<<(sbin& stream, const NamelessTag<T>& t) {
    stream << t.tag.payload;
    return stream;
  }
  
  template<class T>
  sbin& operator>>(sbin& stream, NamelessTag<T> t) {
    stream >> t.tag.payload;
    return stream;
  }
  
  bool operator==(const Tag&, const Tag&);
  
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
     LIST_KIND = nbt_type::LIST,
     COMPOUND_KIND = nbt_type::COMPOUND
    };
}
