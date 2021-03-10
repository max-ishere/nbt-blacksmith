#include "../include/compound.hpp"
namespace blacksmith {
    sbin& operator<<(sbin& stream, const CompoundTag& t) {
    stream << nbt_type::COMPOUND << t.name
	   << t.payload << nbt_type::END;
    return stream;
  } 
  sbin& operator>>(sbin& stream, CompoundTag& t) {
    if (stream.peek() != nbt_type::COMPOUND)
      return stream;
    stream.get();

    stream >> t.name;
    while (stream.peek() != nbt_type::END) {
      shared_ptr<Tag> nested;
      stream >> nested;
      if (nested != nullptr)
	t.payload.push_back(nested);
    }
    stream.get();
    return stream;
  }

  sbin& operator<<(sbin& stream, const vector<shared_ptr<Tag> >& t) {
    for (const shared_ptr<Tag>& tt : t)
      stream << tt;
    return stream;
  }
  
  bool operator==(const CompoundTag&lhs,
		  const CompoundTag&rhs) {
    return (lhs.name == rhs.name && lhs.payload == rhs.payload);
  }

  bool operator==(vector<shared_ptr<Tag> > lhs,
		  vector<shared_ptr<Tag> > rhs) {
    if (lhs.size() != rhs.size())
      return false;

    vector<bool> visited(rhs.size(), false);
    size_t match_count = 0;
    
    for (size_t i = 0; i < lhs.size(); i++)
      for (size_t j = 0; j < rhs.size(); j++) {
	if (visited[j])
	  continue;

	if (lhs[i] == rhs[j]) {
	  visited[j] = true;
	  match_count++;
	}
      }
    if (match_count == lhs.size())
      return true;
    else
      return false;
  }
}
