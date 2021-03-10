#include "sbin.hpp"
#include <fstream>

namespace blacksmith {
  using std::string;
  
  sbin::sbin(const vector<uint8_t>& bin) : bin(bin) {
    it = this->bin.begin();
    if (bin.begin() == bin.end())
      eos_flag = true;
    else
      eos_flag = false;
  }

  sbin::sbin(std::istream_iterator<uint8_t>&start,
	     std::istream_iterator<uint8_t>&end) :
    bin(start, end) {
    it = this->bin.begin();
    if (bin.begin() == bin.end())
      eos_flag = true;
    else
      eos_flag = false;
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

  void sbin::seek(vector<uint8_t>::iterator it) {
    if (bin.begin() <= it && it <= bin.end()) {
      this->it = it;
      eos_flag = false;
    }
  }

  size_t sbin::size() const {
    return bin.size();
  }
  void sbin::resize(size_t n) {
    bin.resize(n);
  }
  
  void sbin::resize(size_t n, uint8_t val) {
    bin.resize(n, val);
  }

  size_t sbin::capacity() const {
    return bin.capacity();
  }

  void sbin::reserve(size_t n) {
    size_t i = distance(bin.begin(), it);
    bin.reserve(n);
    it = bin.begin() + i;
  }
  
  uint8_t sbin::get() {
    if (it < bin.begin() || bin.end() <= it)
      return 0;
    else {
      if (it + 1 == bin.end())
	eos_flag = true;
      return *it++;
    }
  }

  uint8_t sbin::peek() {
    if (it < bin.begin() && bin.end() <= it)
      return 0;
    else
      return *(it);
  }

  bool sbin::operator==(const sbin& other) const {
    return this->bin == other.bin;
  }

  bool sbin::operator!=(const sbin& other) const {
    return this->bin != other.bin;
  }

  const sbin& sbin::operator=(const sbin& other) {
    this->bin = other.bin;

    it = this->bin.begin();
    if (bin.begin() == bin.end())
      eos_flag = true;
    else
      eos_flag = false;
    
    return *this;
  }
  
  const sbin& sbin::operator=(const vector<uint8_t>& other) {
    this->bin = other;

    it = this->bin.begin();
    if (bin.begin() == bin.end())
      eos_flag = true;
    else
      eos_flag = false;
    
    return *this;
  }
  
  void sbin::clear() {
    bin.clear();
    seek(bin.begin());
    eos_flag = false;
  }
  
  bool sbin::empty() {
    return bin.empty();
  }

  sbin::operator bool() {
    return !eos_flag;
  }
  
  bool sbin::eos() {
    return eos_flag;
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
