#include "bigint.hpp"
#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>

bigint::bigint() : _value("0") {}

bigint::bigint(unsigned int value) {
  std::ostringstream oss;
  oss << value;
  _value = oss.str();
}

bigint::bigint(const bigint &other) : _value(other.getter()) {}

bigint &bigint::operator=(const bigint &other) {
  if (this == &other)
    return *this;
  _value = other.getter();
  return *this;
}

bigint::~bigint() {}

std::string bigint::getter() const { return _value; }

void bigint::setter(std::string result) { _value = result; }

bigint operator+(const bigint &a, const bigint &b) {
  bigint _new;
  std::string re;
  std::string _long;
  std::string _short;

  if (a.getter().length() > b.getter().length()) {
    _long = a.getter();
    _short = b.getter();
  } else {
    _long = b.getter();
    _short = a.getter();
  }

  _short.insert(0, _long.length() - _short.length(), '0');

  int plusOne = 0;
  for (int i = _long.length() - 1; i >= 0; --i) {
    int num = (_long[i] - '0') + (_short[i] - '0') + plusOne;
    plusOne = 0;
    if (num > 9) {
      plusOne = 1;
    }
    re += num % 10 + '0';
  }
  if (plusOne)
    re += '1';
  _new.setter(std::string(re.rbegin(), re.rend()));
  return (_new);
}

bigint &operator+=(bigint &a, const bigint &b) {
  a = a + b;
  return a;
}

bigint &bigint::operator++() {
  *this += 1;
  return *this;
}

bigint bigint::operator++(int) {
  bigint tmp = *this;
  *this += 1;
  return tmp;
}

bool operator==(const bigint &a, const bigint &b) {
  return (a.getter() == b.getter());
}

bool operator!=(const bigint &a, const bigint &b) {
  return (a.getter() != b.getter());
}

bool operator>(const bigint &a, const bigint &b) {
  if (a.getter().length() == b.getter().length()) {
    return (a.getter() > b.getter());
  }
  return (a.getter().length() > b.getter().length());
}

bool operator<(const bigint &a, const bigint &b) { return (b > a); }

bool operator>=(const bigint &a, const bigint &b) {
  return ((a == b) || (a > b));
}

bool operator<=(const bigint &a, const bigint &b) {
  return ((a == b) || (a < b));
}

bigint operator<<(const bigint &a, const bigint &b) {
  bigint max;
  std::ostringstream os;
  std::string _limit;
  os << _limit.max_size() - a.getter().length();
  max.setter(os.str());
  if (b > max)
    throw std::overflow_error("Error");
  if (a.getter() == "0")
    return a;
  char *endp = NULL;
  size_t num = strtol(b.getter().c_str(), &endp, 10);
  std::string tmp = a.getter();
  tmp.append(num, '0');
  bigint ret;
  ret.setter(tmp);
  return ret;
}

bigint operator>>(const bigint &a, const bigint &b) {
  errno = 0;
  char *endp = NULL;
  size_t num = strtol(b.getter().c_str(), &endp, 10);
  if (errno == ERANGE) {
    return (bigint(0));
  }
  if (a.getter().length() <= num) {
    return (bigint(0));
  }
  std::string tmp = a.getter();
  tmp.erase(tmp.end() - num, tmp.end());
  bigint ret;
  ret.setter(tmp);
  return (ret);
}

bigint &operator<<=(bigint &a, const bigint &b) {
  a = a << b;
  return a;
}

bigint &operator>>=(bigint &a, const bigint &b) {
  a = a >> b;
  return a;
}

std::ostream &operator<<(std::ostream &os, const bigint &obj) {
  return os << obj.getter();
}
