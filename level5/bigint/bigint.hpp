#ifndef BIGINT_HPP
#define BIGINT_HPP

#include <iostream>
#include <ostream>
#include <string>

class bigint {
private:
  std::string _value;

public:
  bigint();
  bigint(unsigned int ui);
  bigint(const bigint &other);
  bigint &operator=(const bigint &other);
  ~bigint();

  std::string getter() const;
  void setter(std::string result);

  bigint &operator++();
  bigint operator++(int);
};

bigint operator+(const bigint &a, const bigint &b);
bigint &operator+=(bigint &a, const bigint &b);
std::ostream &operator<<(std::ostream &os, const bigint &obj);

bool operator==(const bigint &a, const bigint &b);
bool operator!=(const bigint &a, const bigint &b);
bool operator>(const bigint &a, const bigint &b);
bool operator<(const bigint &a, const bigint &b);
bool operator>=(const bigint &a, const bigint &b);
bool operator<=(const bigint &a, const bigint &b);

bigint operator<<(const bigint &a, const bigint &b);
bigint operator>>(const bigint &a, const bigint &b);
bigint &operator<<=(bigint &a, const bigint &b);
bigint &operator>>=(bigint &a, const bigint &b);
#endif
