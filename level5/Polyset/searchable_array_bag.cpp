#include "searchable_array_bag.hpp"
#include "array_bag.hpp"

searchable_array_bag::searchable_array_bag() : array_bag() {}

searchable_array_bag::searchable_array_bag(const searchable_array_bag &other)
    : array_bag(other) {}

searchable_array_bag &
searchable_array_bag::operator=(const searchable_array_bag &other) {
  if (this == &other)
    return *this;
  array_bag::operator=(other);
  return *this;
}

searchable_array_bag::~searchable_array_bag() {}

bool searchable_array_bag::has(int _target) const {
  for (int i; i < size; i++) {
    if (data[i] == _target)
      return true;
  }
  return false;
}
