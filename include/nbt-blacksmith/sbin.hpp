#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <iterator>

namespace blacksmith {
  using std::vector;
  using std::string;
  using std::shared_ptr;
  
  class sbin {
    vector<uint8_t> bin = { };
    vector<uint8_t>::iterator it = bin.begin();
    
  public:
    sbin() = default;
    sbin(const vector<uint8_t>&);
    sbin(std::istream_iterator<uint8_t>&,
	 std::istream_iterator<uint8_t>&);

    vector<uint8_t>::const_iterator begin() const;
    vector<uint8_t>::const_iterator end() const;
    vector<uint8_t>::const_iterator cur() const;

    vector<uint8_t>::iterator begin();
    vector<uint8_t>::iterator end();
    vector<uint8_t>::iterator cur();

    void seek(vector<uint8_t>::iterator);
    
    template <class InputIterator>
    void append(InputIterator first, InputIterator last);
    
    size_t size() const;
    void resize(size_t);
    void resize(size_t n, uint8_t val);
    
    size_t capacity() const;
    void reserve(size_t n);
    
    template<class T>
    sbin& operator<<(const T& t);

    template<class T>
    sbin& operator>>(T& t);

    bool operator==(const sbin&) const;
    bool operator!=(const sbin&) const;
    const sbin& operator=(const sbin&);
    const sbin& operator=(const vector<uint8_t>&);
    
    uint8_t get();
    uint8_t peek();
    
    void clear();
    bool empty();
    
    operator bool();
    bool eos();
  };

  template<class T>
  sbin& sbin::operator<<(const T& t) {
    size_t bytes = sizeof(T);
    bin.reserve(bin.size() + bytes);
  
    for (int64_t i = bytes - 1; i >= 0; i--) 
      bin.push_back((uint8_t) (t >> (i * 8) & 0xff));
  
    it = bin.begin();
    return *this;
  }
    
  template<class T>
  sbin& sbin::operator>>(T& t) {
    size_t bytes = sizeof(T);
    t = 0;
  
    size_t i = 0;
    for (vector<uint8_t>::iterator end = it + bytes;
	 it < end && it < bin.end(); it++, i++) {
      t <<= 8;
      t |= *it;
    }
    t <<= 8 * (bytes - i);
  
    if (it >= bin.end()) {
      it = bin.end();
    }
    return *this;
  }

  template <class InputIterator>
  void sbin::append(InputIterator first, InputIterator last) {
    bin.reserve(bin.size() + distance(first, last));
    bin.insert(bin.end(), first, last);
    it = bin.begin();
  }

  sbin& operator<<(sbin& stream, const string& str);  
  sbin& operator>>(sbin& stream, string& str);

  sbin& operator<<(sbin& stream, const float& f);  
  sbin& operator>>(sbin& stream, float& f);
  
  sbin& operator<<(sbin& stream, const double& d);  
  sbin& operator>>(sbin& stream, double& d);

  template<class T>
  sbin& operator<<(sbin& stream, const vector<T>& t) {
    stream << (int32_t)t.size();
    for (const T& tt : t) 
      stream << tt;
    return stream;
  }

  template<class T>
  sbin& operator>>(sbin& stream, vector<T>& t) {
    int32_t length = 0;
    stream >> length;
    t.resize(length);

    size_t i = 0;
    for (T& tt : t) {
      stream >> tt;
      i++;
      if (!stream) {
	t.resize(i);
	break;
      }
    }
    return stream;
  }
}
