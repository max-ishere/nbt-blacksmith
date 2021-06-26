#pragma once
#include <string>
#include <vector>
#include <memory>
#include "tag.hpp"
namespace blacksmith {
  using std::string;
  using std::vector;
  using std::shared_ptr;
  
  class CompoundTag : public Tag {
  public:
    vector<shared_ptr<Tag>> payload = { };

    CompoundTag() = default;
    CompoundTag(string name, vector<shared_ptr<Tag>> payload)
      : Tag(name), payload(payload) { }
    CompoundTag(vector<shared_ptr<Tag>> payload)
      : payload(payload) { }
    CompoundTag(const CompoundTag &other) = default;
    CompoundTag(CompoundTag &&other) noexcept = default;
    ~CompoundTag() noexcept = default;

    CompoundTag& operator=(const CompoundTag &other) = default;
    CompoundTag& operator=(CompoundTag &&other) noexcept = default;

    virtual uint8_t kind() const final override {
      return nbt_kind::COMPOUND_KIND;
    }

    virtual uint8_t type() const final override {
      return nbt_type::COMPOUND;

    }
    size_t size() const { return payload.size(); }
  };

  sbin& operator<<(sbin&, const CompoundTag&);
  sbin& operator>>(sbin&, CompoundTag&);

  sbin& operator<<(sbin&, const vector<shared_ptr<Tag> >&);

  bool operator==(const CompoundTag&, const CompoundTag&);

  bool operator==(vector<shared_ptr<Tag> >, vector<shared_ptr<Tag> >);
}
