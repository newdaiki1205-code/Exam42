#include "set.hpp"
#include <cstdlib>

set::set(): _bag(NULL) {}

set::set(searchable_bag& bag): _bag(&bag){}

set::set(const set& other) {
		*this = other;
}

set& set::operator=(const set& other){
		if(this == &other)
				return *this;
		_bag = &other.get_bag();
		return *this;
}

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

searchable_bag& set::get_bag() const {
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

