#pragma once
#include <string>
#include <vector>
#include "tag.hpp"
namespace blacksmith {
  using std::string;
  using std::vector;

  class TypelessList : public Tag {
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
    
    virtual uint8_t type() const final override;
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
    
    virtual uint8_t type() const final override {
      return nbt_type::END;
    }
  };

  sbin& operator<<(sbin&, const shared_ptr<TypelessList>);
  sbin& operator>>(sbin&, shared_ptr<TypelessList>&);

  template<class T>
  sbin& operator<<(sbin& stream, const ListTag<T>& t) {
    stream << t.kind() << t.name << t.type()
	   << NamelessTag<const ListTag<T> >(t);
    return stream;
  }
  
  template<class T>
  sbin& operator>>(sbin& stream, ListTag<T>& t) {
    if (stream.peek() != t.kind())
      return stream;
    auto cur = stream.cur();
    stream.get();

    stream >> t.name;
    if (stream.get() != t.type()) {
      stream.seek(cur);
      return stream;
    }

    stream >> NamelessTag<ListTag<T> >(t);
    return stream;
  }

  sbin& operator<<(sbin& stream, const ListTag<void>& t);
  sbin& operator>>(sbin& stream, ListTag<void>& t);
  
  template<class T>
  bool operator==(const ListTag<T>&lhs,
		  const ListTag<T>&rhs) {
    return (lhs.name == rhs.name &&
	    lhs.payload == rhs.payload);
  }
}
