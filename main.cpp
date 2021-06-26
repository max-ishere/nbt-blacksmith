#include <fstream>
#include <iterator>
#include <vector>
#include <string>
#include <memory>

#include <nbt-blacksmith/ios-bin.hpp>
#include <nbt-blacksmith/nbt.hpp>

using namespace std;
using namespace blacksmith;

int main(int argc, char** argv) {
  std::ios_base::sync_with_stdio(false);
  string filename = "test-files/array.bin";
  if (argc == 2) filename = argv[1];
  
  ifstream file(filename, ios::binary);

  if (!file) {
    cerr << "Could not open file\n";
    exit(1);
  }
  
  file.unsetf(ios::skipws);
  std::istream_iterator<uint8_t> start(file), end;
  sbin stream(start, end);
  
  shared_ptr<Tag> root;
  stream >> root;
  
  if (root == nullptr) {
    cerr << "Root is a nullptr, terminating.\n";
    exit (1);
  }
  cout << root << "\n";
}
