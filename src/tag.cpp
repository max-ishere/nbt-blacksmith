#include "../include/nbt.hpp"
#include <iostream>
#include <iomanip>
namespace blacksmith {
  using std::make_shared;
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
  
  bool operator==(const Tag&, const Tag&) { return false; }
}


