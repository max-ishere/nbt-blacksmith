#pragma once
#include <string>
#include "tag.hpp"
namespace blacksmith {
  using std::string;
  
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
  sbin& operator<<(sbin& stream, const PrimitiveTag<T>& t) {
    stream << t.type() << t.name
	   << NamelessTag<const PrimitiveTag<T> >(t);
    return stream;
  } 
  template<class T>
  sbin& operator>>(sbin& stream, PrimitiveTag<T>& t) {
    if (stream.peek() != t.type())
      return stream;
    stream.get();
    stream >> t.name >> NamelessTag<PrimitiveTag<T> >(t);
    return stream;
  }

  template <class T>
  bool operator==(const PrimitiveTag<T>&lhs,
		  const PrimitiveTag<T>&rhs) {
    return (lhs.name == rhs.name &&
	    lhs.payload == rhs.payload);
  }
}
