#pragma once

namespace blacksmith {
  sbin& operator<<(sbin& stream, const vector<shared_ptr<Tag> >& t);
  
  sbin& operator<<(sbin& stream, const shared_ptr<Tag> t);
  sbin& operator>>(sbin& stream, shared_ptr<Tag>& t);

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

  template<class T>
  sbin& operator<<(sbin& stream, const ArrayTag<T>& t) {
    stream << t.type() << t.name
	   << NamelessTag<const ArrayTag<T> >(t);
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

  template<>
  sbin& operator<<(sbin& stream, const ListTag<void>& t);
  template<>
  sbin& operator>>(sbin& stream, ListTag<void>& t);

  template<>
  uint8_t ListTag<CompoundTag>::type() const;

  sbin& operator<<(sbin&, const CompoundTag&);
  sbin& operator>>(sbin&, CompoundTag&);

  template <class T>
  bool operator==(const PrimitiveTag<T>&lhs,
		  const PrimitiveTag<T>&rhs) {
    return (lhs.name == rhs.name && lhs.payload == rhs.payload);
  }

  template <class T>
  bool operator==(const ArrayTag<T>&lhs,
		  const ArrayTag<T>&rhs) {
    return (lhs.name == rhs.name && lhs.payload == rhs.payload);
  }

  template<class T>
  bool operator==(const ListTag<T>&lhs,
		  const ListTag<T>&rhs) {
    return (lhs.name == rhs.name && lhs.payload == rhs.payload);
  }

  bool operator==(const CompoundTag&, const CompoundTag&);

  bool operator==(vector<shared_ptr<Tag> > lhs,
		  vector<shared_ptr<Tag> > rhs);

  bool operator==(const Tag&, const Tag&);
}
