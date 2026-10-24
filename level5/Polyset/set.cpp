#include "set.hpp"
#include <cstdlib>
#include <stdexcept>

set::set(): _bag(NULL) {}

set::set(searchable_bag& bag): _bag(&bag){}

set::set(const set& other) : _bag(other._bag){}

set& set::operator=(const set& other){
		if(this == &other)
				return *this;
		*this = set(other);
		return *this;
}

set::~set(){}

void set::insert(int value){
		if(!_bag)
				return;
		if(_bag->has(value))
				return;
		_bag->insert(value);
}

void set::insert(int *arr, int size){
	if(!_bag)
				return;
	for(int i = 0; i < size; i++){
				if(!_bag->has(arr[i]))
						_bag->insert(arr[i]);
		}
}

searchable_bag& set::get_bag()  {
		if(!_bag)
				throw std::runtime_error("Error");
		return *_bag;
}

const searchable_bag& set::get_bag() const {
		if(!_bag)
				throw std::runtime_error("Error");
		return *_bag;
}

void set::print() const {
	if(!_bag)
				return;
	_bag->print();
}

bool set::has(int value) const {
	if(!_bag)
				return false;
	return _bag->has(value);
}

void set::clear(){
	if(!_bag)
				return;
	_bag->clear();
}

