#pragma once

namespace blacksmith {
  using std::string;

  template<class T>
  sbin& sbin::operator<<(const T& t) {
    size_t bytes = sizeof(T);
    bin.reserve(bin.size() + bytes);
  
    for (int64_t i = bytes - 1; i >= 0; i--) 
      bin.push_back((uint8_t) (t >> (i * 8) & 0xff));
  
    it = bin.begin();
    eos_flag = false;
  
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
      eos_flag = true;
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
