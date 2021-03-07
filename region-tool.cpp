#include <fstream>
#include <iterator>
#include <vector>
#include <string>
#include <memory>

#include "ios-bin.hpp"
#include "nbt.hpp"

using namespace std;
using namespace blacksmith;

int main(int argc, char** argv) {
  string filename = "files/r.-3.1.mca";
  if (argc == 2) filename = argv[1];
  
  ifstream file(filename, ios::binary);

  if (!file) {
    cerr << "Could not open file\n";
    exit(1);
  }

  clog << "Opened file " << filename << "\n";
  
  file.unsetf(ios::skipws);
  
  std::istream_iterator<uint8_t> start(file), end;
  std::vector<uint8_t> bytes(start, end);
  
  sbin stream(bytes);
  
  int32_t cx = -80, cz = 42;
  int32_t count = 4 * ((cx & 31) + (cz & 31) * 32);
  clog << count << "\n";
  stream.seek(stream.begin() + count);
  
  size_t var_len = 3;
  int32_t offset = 0;
  size_t i = 0;
  for (auto end = stream.cur() + var_len;
       stream.cur() < end && stream.cur() < stream.end(); i++) {
    offset <<= 8;
    offset |= stream.get();
  }
  offset <<= 8 * (var_len - i);
  clog << offset << "\n";
  
  stream.seek(stream.begin() + offset * 0x1000);
  int32_t lenght;
  stream >> lenght;
  assert(stream.get() == 2);

  vector<uint8_t> data(lenght);
  stream >> data;

  std::ofstream output_file("target-chunk", ios::binary);
  output_file.unsetf(ios::skipws);
  
  std::ostream_iterator<uint8_t> output_iterator(output_file);
  std::copy(data.begin(), data.end(), output_iterator);
}

