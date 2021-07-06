#include "nbt-blacksmith/sbin.hpp"
#include <fstream>

namespace blacksmith {
  using std::string;
  
  sbin::sbin(const vector<uint8_t>& bin) : bin(bin) {
    it = this->bin.begin();
  }

  sbin::sbin(std::istream_iterator<uint8_t>&start,
	     std::istream_iterator<uint8_t>&end) :
    bin(start, end) {
    it = this->bin.begin();
  }

  vector<uint8_t>::iterator sbin::begin() {
    return bin.begin();
  }
  
  vector<uint8_t>::iterator sbin::end() {
    return bin.end();
  }
  
  vector<uint8_t>::iterator sbin::cur() {
    return it;
  }

  
  vector<uint8_t>::const_iterator sbin::begin() const {
    return bin.begin();
  }
  
  vector<uint8_t>::const_iterator sbin::end() const {
    return bin.end();
  }
  
  vector<uint8_t>::const_iterator sbin::cur() const {
    return it;
  }

  void sbin::seek(vector<uint8_t>::iterator it) {
    if (bin.begin() <= it && it <= bin.end()) {
      this->it = it;
    }
  }

  size_t sbin::size() const {
    return bin.size();
  }
  void sbin::resize(size_t n) {
    resize(n, 0);
  }
  
  void sbin::resize(size_t n, uint8_t val) {
    size_t old_size = bin.size();
    size_t old_distance = distance(begin(), cur());
    
    bin.resize(n, val);
    
    if (n >= old_size)
      it = bin.begin() + old_distance;
  
    else
      it = bin.begin();
  }

  size_t sbin::capacity() const {
    return bin.capacity();
  }

  void sbin::reserve(size_t n) {
    size_t old_capacity = bin.capacity();
    size_t old_distance = distance(bin.begin(), it);
    bin.reserve(n);

    it = bin.begin() + old_distance;
  }
  
  uint8_t sbin::get() {
    if (bin.begin() <= it && it < bin.end())
      return *it++;
    else 
      return 0;
  }

  uint8_t sbin::peek() {
    if (bin.begin() <= it && it < bin.end())
      return *(it);
    else 
      return 0;
  }

  bool sbin::operator==(const sbin& other) const {
    return this->bin == other.bin;
  }

  bool sbin::operator!=(const sbin& other) const {
    return this->bin != other.bin;
  }

  const sbin& sbin::operator=(const sbin& other) {
    this->bin = other.bin;
    this->it = this->begin() + distance(other.begin(), other.cur());
    
    return *this;
  }
  
  const sbin& sbin::operator=(const vector<uint8_t>& other) {
    this->bin = other;

    it = this->bin.begin();
    return *this;
  }
  
  void sbin::clear() {
    bin.clear();
    seek(bin.begin());
  }
  
  bool sbin::empty() {
    return bin.empty();
  }

  sbin::operator bool() {
    return !eos();
  }
  
  bool sbin::eos() {
    return cur() == end() || empty();
  }
  
  sbin& operator<<(sbin& stream, const string& str) {
    int16_t length = (int16_t) str.length();
    stream.reserve(stream.size() + length + 2);
    
    stream << length;
    stream.append(str.begin(), str.end());
    return stream;
  }
  
  sbin& operator>>(sbin& stream, string& str) {
    int16_t length = 0;
    stream >> length;

    if (stream.cur() + length < stream.end()) {
      str.assign(stream.cur(), stream.cur() + length);
      stream.seek(stream.cur() + length);
    }
    else {
      str.assign(stream.cur(), stream.end());
    }
    return stream;
  }

  sbin& operator<<(sbin& stream, const float& f) {
    stream << *(uint32_t*) &f;
    return stream;
  }
  
  sbin& operator>>(sbin& stream, float& f) {
    stream >> *(uint32_t*) &f;
    return stream;
  }
  
  sbin& operator<<(sbin& stream, const double& d) {
    stream << *(uint64_t*) &d;
    return stream;
  }
  
  sbin& operator>>(sbin& stream, double& d) {
    stream >> *(uint64_t*) &d;
    return stream;
  }
}
