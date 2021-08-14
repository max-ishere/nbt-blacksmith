#pragma once
#include <iostream>
#include <iomanip>
#include <iostream-indenter/indent_facet.hpp>
#include "nbt.hpp"

namespace blacksmith {
  using std::ostream;
  using std::setw;
  using std::setfill;
  using std::hex;
  using std::dec;

  std::ostream& operator<<(ostream& os, const sbin& stream);

  template<class T>
  std::ostream& operator<<(ostream& os, const PrimitiveTag<T>& t) {
    constexpr char names[][4] = { "b", "s", "i", "l",
				  "f", "d", "?", "str" }; 
       
    os << hex << names[t.type() - 1] << " \""<< t.name << "\": "
       << t.payload << "\n" << dec;
    return os;
  }

  template<class T>
  std::ostream& operator<<(std::ostream& os, const ListTag<T>& t) {
    os << hex << (int) t.kind() << "<" << (int) t.type() << ">"
       << " \""<< t.name << "\": {\n";

    os << indent_manip::push;
    for (T val : t.payload)
      os << val << "\n";
  
    os << indent_manip::pop << "}\n" << dec;
    return os;
  }

  template<class T>
  std::ostream& operator<<(std::ostream& os, const ListTag<ArrayTag<T> >& t) {
    os << hex << (int) t.kind() << "<" << (int) t.type() << ">"
       << " \""<< t.name << "\": {\n";

    os << indent_manip::push;
    for (ArrayTag<T> val : t.payload)
      os  << setfill('0') << setw(2) << val;
  
    os << indent_manip::pop <<  "}\n" << dec << setfill(' ');
    return os;
  }
  
  template<class T>
  std::ostream& operator<<(std::ostream& os, const ArrayTag<T>& t) {
    os << hex << (int) t.type()
       << " \""<< t.name << "\": {\n";

    os << indent_manip::push;
    for (T val : t.payload)
      os << setfill('0') << setw(2) << val << "\n";
  
    os << indent_manip::pop << "}\n" << dec << setfill(' ');
    return os;
  }

  ostream& operator<<(ostream& os, const CompoundTag& t);
  ostream& operator<<(ostream& os, const shared_ptr<Tag>& t);
}
