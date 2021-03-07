#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <iterator>

namespace blacksmith {
  using std::vector;
  class sbin {
    vector<uint8_t> bin = { };
    vector<uint8_t>::iterator it = bin.begin();
    bool eos_flag = true;
    
  public:
    sbin() = default;
    sbin(const vector<uint8_t>&);
    sbin(std::istream_iterator<unsigned char>&,
	 std::istream_iterator<unsigned char>&);

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
}
#include "t-sbin.hpp"
