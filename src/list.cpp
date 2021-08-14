#include "nbt-blacksmith/nbt.hpp"
#include <iostream>
#include <iomanip>
namespace blacksmith {
  using std::make_shared;
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
      // #warning "List<List> is diabled."
      // stream.seek(stream.end());
      // return stream;
      
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

  sbin& operator<<(sbin& stream, const ListTag<void>& t) {
    stream << t.kind() << t.name << t.type()
	   << t.length;
#warning "May have missing trailing 0"
    
    return stream;
  }
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
