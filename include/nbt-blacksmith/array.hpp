#pragma once
#include <vector>
#include <string>
#include "tag.hpp"
namespace blacksmith {
  using std::string;
  using std::vector;
  
  template<class T>
  class ArrayTag : public Tag {
  public:
    vector<T> payload = { };

    ArrayTag() = default;
    ArrayTag(string name, vector<T> payload)
      : Tag(name), payload(payload) { }
    ArrayTag(string name)
      : Tag(name) { }
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

  template<class T>
  sbin& operator<<(sbin& stream, const ArrayTag<T>& t) {
    stream << t.type() << t.name;
    const NamelessTag<const ArrayTag<T> > data(t);
    stream << data;

    return stream;
  }  
  template<class T>
  sbin& operator>>(sbin& stream, ArrayTag<T>& t) {
    if (stream.peek() != t.type())
      return stream;
    stream.get();
    stream >> t.name
	   >> NamelessTag<ArrayTag<T> >(t);
    return stream;
  }

   template <class T>
  bool operator==(const ArrayTag<T>&lhs,
		  const ArrayTag<T>&rhs) {
    return (lhs.name == rhs.name &&
	    lhs.payload == rhs.payload);
  }
}
