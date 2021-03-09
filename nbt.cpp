#include "nbt.hpp"
#include <iostream>
#include <iomanip>
#include <execinfo.h>

namespace blacksmith {
  sbin& operator<<(sbin& stream, const shared_ptr<Tag> t) {
    switch (t->kind()) {
    case PRIMITIVE_KIND:
      switch(t->type()) {
      case BYTE:
	stream << dynamic_cast<const PrimitiveTag<uint8_t>&>(*t);
	break;
      case SHORT:
	stream << dynamic_cast<const PrimitiveTag<int16_t>&>(*t);
	break;
      case INT:
	stream << dynamic_cast<const PrimitiveTag<int32_t>&>(*t);
	break;
      case LONG:
	stream << dynamic_cast<const PrimitiveTag<int64_t>&>(*t);
	break;
      case FLOAT:
	stream << dynamic_cast<const PrimitiveTag<float>&>(*t);
	break;
      case DOUBLE:
	stream << dynamic_cast<const PrimitiveTag<double>&>(*t);
	break;
      case STRING:
	stream << dynamic_cast<const PrimitiveTag<string>&>(*t);
	break;
      }
      break;
      
    case ARRAY_KIND:
      switch (t->type()) {
      case BYTE_ARRAY:
	stream << dynamic_cast<const ArrayTag<uint8_t>&>(*t);
	break;
      case INT_ARRAY:
	stream << dynamic_cast<const ArrayTag<int32_t>&>(*t);
	break;
      case LONG_ARRAY:
	stream << dynamic_cast<const ArrayTag<int64_t>&>(*t);
	break;
      }
      break;
      
    case LIST_KIND:
      switch(t->type()) {
      case BYTE:
	stream << dynamic_cast<const ListTag<uint8_t>&>(*t);
	break;
      case SHORT:
	stream << dynamic_cast<const ListTag<int16_t>&>(*t);
	break;
      case INT:
	stream << dynamic_cast<const ListTag<int32_t>&>(*t);
	break;
      case LONG:
	stream << dynamic_cast<const ListTag<int64_t>&>(*t);
	break;
      case FLOAT:
	stream << dynamic_cast<const ListTag<float>&>(*t);
	break;
      case DOUBLE:
	stream << dynamic_cast<const ListTag<double>&>(*t);
	break;
      case BYTE_ARRAY:
	stream << dynamic_cast<const ListTag<ArrayTag<uint8_t>>&>(*t);
	break;
      case STRING:
	stream << dynamic_cast<const ListTag<string>&>(*t);
	break;
      case LIST:
	// not supported, may be coming soon
	break;
      case COMPOUND:
	stream << dynamic_cast<const ListTag<CompoundTag>&>(*t);
	break;
      case INT_ARRAY:
	stream << dynamic_cast<const ListTag<ArrayTag<int32_t>>&>(*t);
	break;
      case LONG_ARRAY:
	stream << dynamic_cast<const ListTag<ArrayTag<int64_t>>&>(*t);
	break;
      }
      break;
      
    case COMPOUND_KIND:
      stream << dynamic_cast<const CompoundTag&>(*t);
      break;
    }
    return stream;
  }
  sbin& operator>>(sbin& stream, shared_ptr<Tag>& t) {
    using std::cerr;
    using std::clog;
    using std::setw;
    using std::setfill;
    using std::hex;
    using std::dec;
    
    switch (stream.peek()) {
    case BYTE: {
      PrimitiveTag<uint8_t> tag;
      stream >> tag;
      t = make_shared<PrimitiveTag<uint8_t> >(tag);
      break;
    }
    case SHORT: {
      PrimitiveTag<int16_t> tag;
      stream >> tag;
      t = make_shared<PrimitiveTag<int16_t> >(tag);
      break;
    }
    case INT: {
      PrimitiveTag<int32_t> tag;
      stream >> tag;
      t = make_shared<PrimitiveTag<int32_t> >(tag);
      break;
    }
    case LONG: {
      PrimitiveTag<int64_t> tag;
      stream >> tag;
      t = make_shared<PrimitiveTag<int64_t> >(tag);
      break;
    }
    case FLOAT: {
      PrimitiveTag<float> tag;
      stream >> tag;
      t = make_shared<PrimitiveTag<float> >(tag);
      break;
    }
    case DOUBLE: {
      PrimitiveTag<double> tag;
      stream >> tag;
      t = make_shared<PrimitiveTag<double> >(tag);
      break;
    }
    case STRING: {
      PrimitiveTag<string> tag;
      stream >> tag;
      t = make_shared<PrimitiveTag<string> >(tag);
      break;
    }
      
    case BYTE_ARRAY: {
      ArrayTag<uint8_t> tag;
      stream >> tag;
      t = make_shared<ArrayTag<uint8_t> >(tag);
      break;
    } 
    case INT_ARRAY: {
      ArrayTag<int32_t> tag;
      stream >> tag;
      t = make_shared<ArrayTag<int32_t> >(tag);
      break;
    } 
    case LONG_ARRAY: {
      ArrayTag<int64_t> tag;
      stream >> tag;
      t = make_shared<ArrayTag<int64_t> >(tag);
      break;
    }
       
    case LIST: {
      shared_ptr<TypelessList> tag;
      stream >> tag;
      t = tag;
      break;
    }
       
    case COMPOUND: {
      CompoundTag tag;
      stream >> tag;
      t = make_shared<CompoundTag>(tag);
      break;
    }
      
    default:
      cerr << "Badbyte at position 0x" << hex
	   << distance(stream.begin(), stream.cur())
	   << " 0x" << setw(2) << setfill('0')
	   << (int) stream.peek()
	   << setfill(' ') << dec << "\nExpected Tag ID.\n";
      stream.seek(stream.end());
    }
    return stream;
  }

  sbin& operator<<(sbin& stream, const shared_ptr<TypelessList> t) {
    return stream;
  }
  sbin& operator>>(sbin& stream, shared_ptr<TypelessList>& t) {
    using std::cerr;
    using std::clog;
    using std::setw;
    using std::setfill;
    using std::hex;
    using std::dec;
    
    auto cur = stream.cur();
    stream.get();

    int16_t name_len = 0;
    stream >> name_len;
    stream.seek(stream.cur() + name_len);

    uint8_t type = stream.get();
    stream.seek(cur);
      
    switch(type) {
    case BYTE: {
      ListTag<uint8_t> tag;
      stream >> tag;
      t = make_shared<ListTag<uint8_t>>(tag);
      break;
    }
    case SHORT: {
      ListTag<int16_t> tag;
      stream >> tag;
      t = make_shared<ListTag<int16_t>>(tag);
      break;
    }
    case INT: {
      ListTag<int32_t> tag;
      stream >> tag;
      t = make_shared<ListTag<int32_t>>(tag);
      break;
    }
    case LONG: {
      ListTag<int64_t> tag;
      stream >> tag;
      t = make_shared<ListTag<int64_t>>(tag);
      break;
    }
    case FLOAT: {
      ListTag<float> tag;
      stream >> tag;
      t = make_shared<ListTag<float>>(tag);
      break;
    }
    case DOUBLE: {
      ListTag<double> tag;
      stream >> tag;
      t = make_shared<ListTag<double>>(tag);
      break;
    }
    case BYTE_ARRAY: {
      ListTag<ArrayTag<uint8_t> > tag;
      stream >> tag;
      t = make_shared<ListTag<ArrayTag<uint8_t> > >(tag);
      break;
    }
    case STRING: {
      ListTag<string> tag;
      stream >> tag;
      t = make_shared<ListTag<string> >(tag);
      break;
    }
    case LIST: {
      // REMOVE, work in progress!
      // This will cause bugs because it skips the rest of the data.
      stream.seek(stream.end());
      return stream;
      
      ListTag<shared_ptr<TypelessList> > tag;
      stream >> tag;
      t = make_shared<ListTag<shared_ptr<TypelessList> > >(tag);
      break;
    }
    case COMPOUND: {
      ListTag<CompoundTag> tag;
      stream >> tag;
      t = make_shared<ListTag<CompoundTag> >(tag);
      break;
    }
    case INT_ARRAY: {
      ListTag<ArrayTag<int32_t> > tag;
      stream >> tag;
      t = make_shared<ListTag<ArrayTag<int32_t> > >(tag);
      break;
    }
    case LONG_ARRAY: {
      ListTag<ArrayTag<int64_t> > tag;
      stream >> tag;
      t = make_shared<ListTag<ArrayTag<int64_t> > >(tag);
      break;
    }
    case END: {
      ListTag<void> tag;
      stream >> tag;
      t = make_shared<ListTag<void> >(tag);
      break;
    } 
    default: {
      int32_t is_null = 0;
      stream.seek(cur + 4 + name_len);
      stream >> is_null;
      if (is_null == 0 && type == 0) {
	t = nullptr;
	clog << "Found an empty list at position 0x" << hex
	     << distance(stream.begin(), cur) << "\n";
      }
      else {
	cerr << "Badbyte at position 0x" << hex
	     << distance(stream.begin(), stream.cur() + name_len + 3)
	     << " 0x" << setw(2) << setfill('0') 
	     << (int) type << dec
	     << setfill(' ') << "\nExpected List Type\n";
	stream.seek(stream.end());
      }
      break;
    }
    }
    return stream;
  }

  template<>
  sbin& operator<<(sbin& stream, const ListTag<void>& t) {
    stream << t.kind() << t.name << t.type()
	   << t.length;
    return stream;
  }
  template<>
  sbin& operator>>(sbin& stream, ListTag<void>& t) {
    if (stream.peek() != t.kind())
      return stream;
    auto cur = stream.cur();
    stream.get();

    stream >> t.name;
    if (stream.get() != t.type()) {
      stream.seek(cur);
      return stream;
    }

    stream >> t.length;
    return stream;
  }
  
  sbin& operator<<(sbin& stream, const CompoundTag& t) {
    stream << nbt_type::COMPOUND << t.name
	   << t.payload << nbt_type::END;
    return stream;
  } 
  sbin& operator>>(sbin& stream, CompoundTag& t) {
    if (stream.peek() != nbt_type::COMPOUND)
      return stream;
    stream.get();

    stream >> t.name;
    while (stream.peek() != nbt_type::END) {
      shared_ptr<Tag> nested;
      stream >> nested;
      if (nested != nullptr)
	t.payload.push_back(nested);
    }
    stream.get();
    return stream;
  }

  sbin& operator<<(sbin& stream, const vector<shared_ptr<Tag> >& t) {
    for (const shared_ptr<Tag>& tt : t)
      stream << tt;
    return stream;
  }
  
  bool operator==(const CompoundTag&lhs,
		  const CompoundTag&rhs) {
    return (lhs.name == rhs.name && lhs.payload == rhs.payload);
  }

  bool operator==(vector<shared_ptr<Tag> > lhs,
		  vector<shared_ptr<Tag> > rhs) {
    if (lhs.size() != rhs.size())
      return false;

    vector<bool> visited(rhs.size(), false);
    size_t match_count = 0;
    
    for (size_t i = 0; i < lhs.size(); i++)
      for (size_t j = 0; j < rhs.size(); j++) {
	if (visited[j])
	  continue;

	if (lhs[i] == rhs[j]) {
	  visited[j] = true;
	  match_count++;
	}
      }
    if (match_count == lhs.size())
      return true;
    else
      return false;
  }

  bool operator==(const Tag&, const Tag&) { return false; }
  
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

  template<>
  uint8_t ListTag<uint8_t>::type() const {
    return nbt_type::BYTE;
  }

  template<>
  uint8_t ListTag<int16_t>::type() const {
    return nbt_type::SHORT;
  }

  template<>
  uint8_t ListTag<int32_t>::type() const {
    return nbt_type::INT;
  }

  template<>
  uint8_t ListTag<int64_t>::type() const {
    return nbt_type::LONG;
  }

  template<>
  uint8_t ListTag<float>::type() const {
    return nbt_type::FLOAT;
  }

  template<>
  uint8_t ListTag<double>::type() const {
    return nbt_type::DOUBLE;
  }

  template<>
  uint8_t ListTag<ArrayTag<uint8_t> >::type() const {
    return nbt_type::BYTE_ARRAY;
  }

  template<>
  uint8_t ListTag<ArrayTag<int32_t> >::type() const {
    return nbt_type::INT_ARRAY;
  }

  template<>
  uint8_t ListTag<ArrayTag<int64_t> >::type() const {
    return nbt_type::LONG_ARRAY;
  }

  template<>
  uint8_t ListTag<string>::type() const {
    return nbt_type::STRING;
  }

  template<>
  uint8_t ListTag<shared_ptr<TypelessList> >::type() const {
    return nbt_type::LIST;
  }
  
  template<>
  uint8_t ListTag<CompoundTag>::type() const {
    return nbt_type::COMPOUND;
  }
}
