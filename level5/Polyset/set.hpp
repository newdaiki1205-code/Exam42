#ifndef SET_HPP
# define SET_HPP

#include "searchable_bag.hpp"

class set {
		private:
				searchable_bag* _bag;

		public:
				set();
				set(searchable_bag& bag);
				set(const set& other);
				set& operator=(const set& other);
				~set();
				
				void insert(int value);
				void insert(int *arr, int size);
				searchable_bag& get_bag() ;
				const searchable_bag& get_bag() const;
				void print() const;
				bool has(int value) const;
				void clear();
};
				

#endif

