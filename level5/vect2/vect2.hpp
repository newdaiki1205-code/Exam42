#ifndef VECT2_HPP
# define VECT2_HPP

class vect2 {
		private:
				int _a;
				int _b;

		public:
				vect2();
				vect2(int a, int b);
				vect2(const vect2& other);
				vect2& operator=(const vect2& other);
				~vect2();

				int get_a() const;
				int get_b() const;

				vect2& operator++();
				vect2 operator++(int);
				vect2& operator--();
				vect2 operator--(int);
				int& operator[](int index);
				int operator[](int index) const;
};

std::ostream& operator<<(std::ostream& os, const vect2& obj);
vect2& operator+=(vect2& left, const vect2& right);
vect2& operator-=(vect2& left, const vect2& right);
vect2& operator*=(vect2& left, const vect2& right);
vect2 operator+(const vect2& left, const vect2& right);
vect2 operator-(const vect2& left, const vect2& right);
vect2 operator*(const vect2& left, const vect2& right);
bool operator==(const vect2& left, const vect2& right);
bool operator!=(const vect2& left, const vect2& right);

#endif
