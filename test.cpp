#include "test.hpp"

using namespace std;
using namespace blacksmith;

int main() {
  std::ios_base::sync_with_stdio(false);
  cout << hex;
  run_all_tests();
  cout << "\e[30;103mPassed\e[0m" << endl;
  return 0;
}

void run_all_tests() {
  sbin_test();
  rtti_test();
  parsing_nbt();
}

void sbin_test() {
  sbin_manip_stress();
  sbin_primitive();
  sbin_float();
  sbin_vector();
  sbin_string();
}

void rtti_test() {
  rtti_etc();  
  rtti_primitive();
  rtti_array();
}

void sbin_manip_stress() {
  sbin stream;

  assert(stream.cur() == stream.begin());
  
  stream.reserve(10);
  assert(stream.cur() == stream.begin());
  assert(stream.size() == 0);
  assert(stream.capacity() == 10);

  stream.seek(stream.begin() + 7);
  assert(stream.size() == 0);
  assert(stream.capacity() == 10);
  assert(stream.cur() == stream.begin());
  assert(stream.begin() == stream.end());
  assert(stream.empty());

  stream.resize(5, 0xea);
  stream.seek(stream.begin() + 3);
  stream.seek(stream.begin() - 1);
  assert(stream.cur() == stream.begin() + 3);
  assert(*stream.cur() == 0xea);
  assert(stream.get() == 0xea);
  assert(stream.peek() == 0xea);

  stream.resize(3);
  stream.seek(stream.begin() + 2);
  assert(!stream.eos());
  assert(stream.get() == 0xea);
  assert(!stream);

  string name = "Hello.";
  stream.append(name.begin(), name.end());
  assert(stream.size() == 3 + name.size());
  
  stream.clear();
  assert(stream.size() == 0);
  assert(stream.get() == 0);

  vector<uint8_t> v0({ });
  vector<uint8_t> v1({ 0x01 });
  vector<uint8_t> v2({ 0x00, 0x01 });

  sbin original(v0), copy(v0);
  assert(original == copy);

  copy = original = v1;
  assert(original == copy);

  original = v2;
  assert(original != copy);

  copy = v2;
  assert(original == copy);
}

void sbin_primitive() {
  sbin stream;
  uint8_t byte = 0xab;
  int16_t _short = 0x1234;
  
  stream << byte;
  stream << _short << byte;

  _short = 0;
  byte = 0;
  
  stream.seek(stream.begin() + 2);
  stream >> _short;
  
  assert(_short == 0x34ab);
  stream.seek(stream.end() - 1);
  stream >> _short;
  
  assert((uint16_t)_short == 0xab00);
  assert(stream.size() == 4);

  stream.seek(stream.begin());
}

void sbin_float() {
  sbin stream;
  float f = 0x43;
  double d = 0x12;
  
  stream << d << f;
  d = 0;
  f = 0;
  stream >> d >> f;

  assert(f == 0x43);
  assert(d == 0x12);
}

void sbin_vector() {
  sbin stream;
  vector<int16_t> v = { 0x0001, 0x0002, 0x0003 };

  stream << v;
  v.clear();
  stream >> v;
  
  assert(v.size() == 3);
  assert(v[0] == 1);
  assert(v[1] == 2);
  assert(v[2] == 3);
}

void sbin_string() {
  sbin stream;
  string
    str_test = "Hello World!",
    empty =    "",
    ch1 =      "F",
    dest =     "very very very lololong test gibberish";
  
  stream << str_test << empty << ch1;
  assert(stream.size() == str_test.length() + 2 + 2 + 3);
  
  stream >> dest;
  assert(dest == str_test);
  
  stream >> dest;
  assert(dest == empty);

  stream >> dest;
  assert(dest == ch1);
}

void rtti_primitive() {
  shared_ptr<Tag> tag;
  
  tag = make_shared<PrimitiveTag<uint8_t>>();
  assert(tag->kind() == PRIMITIVE_KIND);
  assert(tag->type() == BYTE);
  
  tag = make_shared<PrimitiveTag<int16_t>>();
  assert(tag->kind() == PRIMITIVE_KIND);
  assert(tag->type() == SHORT);
  
  tag = make_shared<PrimitiveTag<int32_t>>();
  assert(tag->kind() == PRIMITIVE_KIND);
  assert(tag->type() == INT);
  
  tag = make_shared<PrimitiveTag<int64_t>>();
  assert(tag->kind() == PRIMITIVE_KIND);
  assert(tag->type() == LONG);

  
  tag = make_shared<PrimitiveTag<float>>();
  assert(tag->kind() == PRIMITIVE_KIND);
  assert(tag->type() == FLOAT);

  tag = make_shared<PrimitiveTag<double>>();
  assert(tag->kind() == PRIMITIVE_KIND);
  assert(tag->type() == DOUBLE);

  
  tag = make_shared<PrimitiveTag<string>>();
  assert(tag->kind() == PRIMITIVE_KIND);
  assert(tag->type() == STRING);
}

void rtti_array() {
  shared_ptr<Tag> tag;
  
  tag = make_shared<ArrayTag<uint8_t>>();
  assert(tag->kind() == ARRAY_KIND);
  assert(tag->type() == BYTE_ARRAY);

  tag = make_shared<ArrayTag<int32_t>>();
  assert(tag->kind() == ARRAY_KIND);
  assert(tag->type() == INT_ARRAY);

  tag = make_shared<ArrayTag<int64_t>>();
  assert(tag->kind() == ARRAY_KIND);
  assert(tag->type() == LONG_ARRAY);
}

void rtti_list() {
  shared_ptr<Tag> tag;

  tag = make_shared<ListTag<uint8_t>>();
  assert(tag->kind() == LIST_KIND);
  assert(tag->type() == BYTE);

  tag = make_shared<ListTag<int16_t>>();
  assert(tag->kind() == LIST_KIND);
  assert(tag->type() == SHORT);

  tag = make_shared<ListTag<int32_t>>();
  assert(tag->kind() == LIST_KIND);
  assert(tag->type() == INT);

  tag = make_shared<ListTag<int64_t>>();
  assert(tag->kind() == LIST_KIND);
  assert(tag->type() == LONG);

  tag = make_shared<ListTag<float>>();
  assert(tag->kind() == LIST_KIND);
  assert(tag->type() == FLOAT);

  tag = make_shared<ListTag<double>>();
  assert(tag->kind() == LIST_KIND);
  assert(tag->type() == DOUBLE);

  tag = make_shared<ListTag<ArrayTag<uint8_t>>>();
  assert(tag->kind() == LIST_KIND);
  assert(tag->type() == BYTE_ARRAY);

  tag = make_shared<ListTag<string>>();
  assert(tag->kind() == LIST_KIND);
  assert(tag->type() == STRING);

  tag = make_shared<ListTag<shared_ptr<TypelessList> > >();
  assert(tag->kind() == LIST_KIND);
  assert(tag->type() == LIST);
  
  tag = make_shared<ListTag<CompoundTag>>();
  assert(tag->kind() == LIST_KIND);
  assert(tag->type() == COMPOUND);

  tag = make_shared<ListTag<ArrayTag<int32_t>>>();
  assert(tag->kind() == LIST_KIND);
  assert(tag->type() == INT_ARRAY);

  tag = make_shared<ListTag<ArrayTag<int64_t>>>();
  assert(tag->kind() == LIST_KIND);
  assert(tag->type() == LONG_ARRAY);
}

void rtti_etc() {
  shared_ptr<Tag> tag = make_shared<Tag>();
  assert(tag->kind() == 0);
  assert(tag->type() == 0);

  tag = make_shared<CompoundTag>();
  assert(tag->kind() == COMPOUND_KIND);
  assert(tag->type() == COMPOUND);
}

void parsing_nbt() {
  parsing_primitive();
  parsing_array();
  parsing_list();
}

void parsing_primitive() {
  ifstream file("files/tests/primitives.bin", ios::binary);
  assert(file);
  file.unsetf(ios::skipws);
  
  istream_iterator<uint8_t> start(file), end;
  sbin stream(start, end), target;
  
  shared_ptr<Tag> root;
  while (stream) {
    stream >> root;
    target << root;
  }
  assert(stream == target);
}

void parsing_array() {
  ifstream file("files/tests/array.bin", ios::binary);
  assert(file);
  file.unsetf(ios::skipws);
  
  istream_iterator<uint8_t> start(file), end;
  sbin stream(start, end), target;
  
  shared_ptr<Tag> root;
  while (stream) {
    stream >> root;
    target << root;
  }
  assert(stream == target);
}

void parsing_list() {
  ifstream file("files/tests/list.bin", ios::binary);
  assert(file);
  file.unsetf(ios::skipws);
  
  istream_iterator<uint8_t> start(file), end;
  sbin stream(start, end), target;
  
  shared_ptr<Tag> root;
  while (stream) {
    stream >> root;
    target << root;
  }
  assert(stream == target);
}

