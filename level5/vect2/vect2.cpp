#include <iostream>
#include "vect2.hpp"

vect2::vect2() : _a(0), _b(0){}

vect2::vect2(int a, int b) : _a(a), _b(b) {}

vect2::vect2(const vect2& other) : _a(other._a), _b(other._b) {}

vect2& vect2::operator=(const vect2& other){
		if(*this == other)
				return *this;
		_a = other._a;
		_b = other._b;
		return *this;
}

vect2::~vect2(){}

int vect2::get_a() const {
		return _a;
}

int vect2::get_b() const {
		return _b;
}

vect2& vect2::operator++(){
		_a = _a + 1;
		_b = _b + 1;
		return *this;
}

vect2 vect2::operator++(int){
		vect2 tmp(*this);
		_a = _a + 1;
		_b = _b + 1;

		return tmp;
}

vect2& vect2::operator--(){
		_a = _a - 1;
		_b = _b - 1;
		return *this;
}

vect2 vect2::operator--(int){
		vect2 tmp(*this);
		_a = _a - 1;
		_b = _b - 1;

		return tmp;
}

int& vect2::operator[](int index){
		if(index == 0)
				return _a;
		return _b;
}

int vect2::operator[](int index) const{
		if(index == 0)
				return _a;
		return _b;
}


std::ostream& operator<<(std::ostream& os, const vect2& obj){
		os << "{" << obj.get_a() << ", " << obj.get_b() << std::endl;
		return os;
}


vect2 operator+(const vect2& left, const vect2& right){
		int tmp_a = left.get_a() + right.get_a();
		int tmp_b = left.get_b() + right.get_b();
		vect2 tmp(tmp_a, tmp_b);
		return(tmp);
}

vect2 operator-(const vect2& left, const vect2& right){
		int tmp_a = left.get_a() - right.get_a();
		int tmp_b = left.get_a() - right.get_b();
		vect2 tmp(tmp_a, tmp_b);
		return(tmp);
}

vect2 operator*(const vect2& left, const vect2& right){
		int tmp_a = left.get_a() * right.get_a();
		int tmp_b = left.get_b() * right.get_b();
		vect2 tmp(tmp_a, tmp_b);
		return(tmp);
}


vect2& operator+=(vect2& left, const vect2& right){
		left = left + right;
		return left;
}

vect2& operator-=(vect2& left, const vect2& right){
		left = left - right;
		return left;
}

vect2& operator*=(vect2& left, const vect2& right){
		left = left * right;
		return left;
}

bool operator==(const vect2& left, const vect2& right){
		if (left.get_a() == right.get_a() && left.get_b() == right.get_b())
				return true;
		return false;
}

bool operator!=(const vect2& left, const vect2& right){
		return (!(left == right));
}











