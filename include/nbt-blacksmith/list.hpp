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
    ListTag(string name)
      : TypelessList(name) { }
    ListTag(vector<T> payload)
      : payload(payload) { }
    ListTag (std::initializer_list<T> payload)
      : payload(payload) { }
    ListTag(const ListTag &other) = default;
    ListTag(ListTag &&other) noexcept = default;
    ~ListTag() noexcept = default;

    ListTag& operator=(const ListTag &other) = default;
    ListTag& operator=(ListTag &&other) noexcept = default;

    int32_t size() { return static_cast<int32_t>(payload.size());}
    T operator[](int32_t i) { return payload[i]; }
    void push_back(const T &t) { payload.push_back(t); }
    
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
  sbin& operator<<(sbin& stream,
		   const NamelessTag<const ListTag<ListTag<T> > > &t) {
    stream << t.tag.type()
	   << (int32_t) t.tag.payload.size();
    for (const ListTag<T> &i : t.tag.payload) {
      const NamelessTag<const ListTag<T> > const_i (i);
      stream << const_i;
    }
    return stream;
  }
  
  template<class T>
  sbin& operator<<(sbin& stream,
		   const NamelessTag<const ListTag<T> > &t) {
    stream << t.tag.type()
	   << (int32_t) t.tag.payload.size();
    for (const T &i : t.tag.payload)
      stream << i;
    return stream;
  }

  template<class T>
  sbin& operator<<(sbin& stream, const NamelessTag<ListTag<T> >& t) {
    stream << t.tag.type() << t.tag.payload;
    return stream;
  }
  
  template<class T>
  sbin& operator<<(sbin& stream, const ListTag<T>& t) {
    stream << t.kind() << t.name;
    const NamelessTag<const ListTag<T> > data(t);
    stream << data;
    return stream;
  }

  template<class T>
  sbin& operator>>(sbin& stream, NamelessTag<ListTag<ListTag<T> > > &t) {
    if (stream.peek() != t.tag.type())
      return stream;
    stream.get();

    int32_t size = 0;
    stream >> size;
    for (int32_t i = 0; i < size; i++) {
      ListTag<T> tt;
      NamelessTag<ListTag<T> > nameless_tt(tt);
      stream >> nameless_tt;
      t.tag.push_back(tt);
    }
    return stream;
  }

  template<class T>
  sbin& operator>>(sbin& stream, NamelessTag<ListTag<T> > &t) {   
    if (stream.peek() != t.tag.type())
      return stream;
    stream.get();

    stream >> t.tag.payload;
    
    return stream;
  }
  
  template<class T>
  sbin& operator>>(sbin& stream, ListTag<T>& t) {
    if (stream.peek() != t.kind())
      return stream;
    
    auto cur = stream.cur();
    stream.get();

    string name;
    stream >> name;
    if (stream.peek() != t.type()) {
      stream.seek(cur);
      return stream;
    }
    
    t.name = name;
    NamelessTag<ListTag<T> > nameless_t(t);
    
    stream >> nameless_t;
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

  template<class T>
  uint8_t ListTag<T >::type() const {
    return nbt_type::LIST;
  }
  
}
