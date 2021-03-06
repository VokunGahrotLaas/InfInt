#ifndef INFINT_HPP
#define INFINT_HPP

// C++ std lib
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <limits>
#include <exception>

// C std lib
#include <cmath>


class InfIntFullDivResult;


class InfInt {
public:
	typedef std::vector<bool>::size_type size_type;
	
	InfInt(void); // init to +0
	InfInt(const InfInt& other); // copy
	InfInt(InfInt&& other); // move
	virtual ~InfInt(void) = default;

	// from ints
	InfInt(int other);
	InfInt(unsigned long long other);
	template <typename T>
	InfInt(T other);

	// from strings
	InfInt(std::string other);							// base 10
	InfInt(std::string other, int base);				// custom base from 2 to 62
	
	bool sign(void) const;
	bool get(const size_type& pos) const;
	size_type size(void) const;
	// complements //
	InfInt& ones_complement(void);
	InfInt& twos_complement(void);
	// casting //
	std::string Bstr(void) const;
	std::string Bstr(size_type str_size) const;
	std::string str(int base) const;
	std::string str(void) const;
	template <class T> T to_int(void) const;
	template <class T> T to_int_safe(void) const;
	// 2 in 1 operator //
	static InfIntFullDivResult fulldiv(const InfInt& a, const InfInt& b);
	// operator //
	// equal
	InfInt& operator=(const InfInt& other);
	// cmp
	bool operator==(const InfInt& other) const;
	bool operator!=(const InfInt& other) const;
	bool operator>(const InfInt& other) const;
	bool operator<=(const InfInt& other) const;
	bool operator<(const InfInt& other) const;
	bool operator>=(const InfInt& other) const;
	// unary
	InfInt operator+(void) const;
	InfInt operator-(void) const;
	// add
	InfInt operator+(const InfInt& other) const;
	InfInt& operator+=(const InfInt& other);
	InfInt& operator++(void);
	InfInt operator++(int);
	// sub
	InfInt operator-(const InfInt& other) const;
	InfInt& operator-=(const InfInt& other);
	InfInt& operator--(void);
	InfInt operator--(int);
	// mul
	InfInt operator*(const InfInt& other) const;
	InfInt& operator*=(const InfInt& other);
	// div
	InfInt operator/(const InfInt& other) const;
	InfInt& operator/=(const InfInt& other);
	// mod
	InfInt operator%(const InfInt& other) const;
	InfInt& operator%=(const InfInt& other);
	// bitwise
	InfInt operator&(const InfInt& other) const;
	InfInt& operator&=(const InfInt& other);
	InfInt operator|(const InfInt& other) const;
	InfInt& operator|=(const InfInt& other);
	InfInt operator^(const InfInt& other) const;
	InfInt& operator^=(const InfInt& other);
	InfInt operator<<(size_type other) const;
	InfInt& operator<<=(size_type other);
	InfInt operator>>(size_type other) const;
	InfInt& operator>>=(size_type other);
	// Static Attributes //
	static const InfInt neg_one;
	static const InfInt zero;
	static const InfInt pos_one;
protected:
	InfInt& clean(void);
	// Attributes //
	std::vector<bool> m_number;
	bool m_sign;
};

template <class T>
T InfInt::to_int(void) const {
	size_type bits_in_T = sizeof(T) * 8;
	T tmp = 0;
	for (size_type i = 0; i < bits_in_T; i++)
		tmp |= static_cast<T>(this->get(i)) << i;
	return tmp;
}

template <class T>
T InfInt::to_int_safe(void) const {
	size_type bits_in_T = sizeof(T) * 8;
	if (bits_in_T < this->size())
		throw std::overflow_error("template <class T> T InfInt::to_int_safe(void) const: Given type is too litle!");
	if (this->sign())
		if (static_cast<T>(-1) >= static_cast<T>(0))
			throw std::overflow_error("template <class T> T InfInt::to_int_safe(void) const: Given type do not have the right signess!");
	T tmp = 0;
	for (size_type i = 0; i < bits_in_T; i++)
		tmp |= static_cast<T>(this->get(i)) << i;
	return tmp;
}

// Outside the class //

InfInt operator "" _infint(unsigned long long other); // alows the use of the macro _infint to transform a unsigned long long to an InfInt
std::ostream& operator<<(std::ostream& out, const InfInt& infint);
std::istream& operator>>(std::istream& in, InfInt& infint);

namespace std {

template<>
class numeric_limits<InfInt> {
public:
	static constexpr bool is_specialized = true;
	static constexpr bool is_signed = true;
	static constexpr bool is_integer = true;
	static constexpr bool is_exact = true;
	static constexpr bool has_infinity = false;
	static constexpr bool has_quiet_NaN = false;
	static constexpr bool has_signaling_NaN = false;
	static constexpr std::float_denorm_style has_denorm = std::denorm_absent;
	static constexpr bool has_denorm_loss = false;
	static constexpr std::float_round_style round_style = std::round_toward_zero;
	static constexpr bool is_iec559 = false;
	static constexpr bool is_bounded = false;
	static constexpr bool is_modulo = false;
	static constexpr int digits = std::numeric_limits<int>::max();
	static constexpr int digits10 = std::numeric_limits<InfInt>::digits * std::log10(2);
	static constexpr int max_digits10 = 0;
	static constexpr int min_exponent = 0;
	static constexpr int min_exponent10 = 0;
	static constexpr int max_exponent = 0;
	static constexpr int max_exponent10 = 0;
	static constexpr bool traps = true;
	static constexpr bool tinyness_before = false;
};

} // std namespace



// InfInt libs
#include "InfIntResult.hpp"

InfInt::InfInt(void):
	m_number({false}),
	m_sign(false)
{
	//
}

InfInt::InfInt(const InfInt& other):
	m_number(other.m_number),
	m_sign(other.m_sign)
{
	//
}

InfInt::InfInt(InfInt&& other):
	m_number(std::move(other.m_number)),
	m_sign(std::move(other.m_sign))
{
	other.m_number = {0};
	other.m_sign = false;
}

InfInt::InfInt(int other) {
	if (other >= 0) {
		if (other == 0) {
			this->m_sign = false;
			this->m_number.push_back(false);
			return;
		}
		this->m_sign = false;
		while (other != 0) {
			this->m_number.push_back(static_cast<bool>(other & 1));
			other >>= 1;
		}
	} else {
		if (other == -1) {
			this->m_sign = true;
			this->m_number.push_back(true);
			return;
		}
		this->m_sign = true;
		while (other != -1) {
			this->m_number.push_back(static_cast<bool>(other & 1));
			other >>= 1;
		}
	}
}

InfInt::InfInt(unsigned long long other) {
	if (other == 0ull) {
		this->m_sign = false;
		this->m_number.push_back(false);
		return;
	}
	this->m_sign = false;
	while (other != 0ull) {
		this->m_number.push_back(static_cast<bool>(other & 1ull));
		other >>= 1ull;
	}
}

template <typename T>
InfInt::InfInt(T other) {
	static_assert(std::numeric_limits<T>::is_integer);
	T zero = 0;
	if (other >= zero) {
		if (other == zero) {
			this->m_sign = false;
			this->m_number.push_back(false);
			return;
		}
		this->m_sign = false;
		while (other != zero) {
			this->m_number.push_back(static_cast<bool>(other & 1));
			other >>= 1;
		}
	} else {
		T minus_one = -1;
		if (other == minus_one) {
			this->m_sign = true;
			this->m_number.push_back(true);
			return;
		}
		this->m_sign = true;
		while (other != minus_one) {
			this->m_number.push_back(static_cast<bool>(other & 1));
			other >>= 1;
		}
	}
}

InfInt::InfInt(std::string other) {
	if (other.empty()) {
		this->m_sign = false;
		this->m_number.push_back(false);
		return;
	} if (other.front() == '-') {
		this->m_sign = true;
		other.assign(other, 1, other.size());
	} else if (other.front() == '+') {
		this->m_sign = false;
		other.assign(other, 1, other.size());
	} else
		this->m_sign = false;
	this->m_number.reserve(other.size());
	while (other.size() > 0) {
		this->m_number.push_back(other.back() != '0');
		other.pop_back();
	}
	this->clean();
}

InfInt::InfInt(std::string other, int base) {
	if (base < 2 || base > 62)
		throw std::domain_error("InfInt::InfInt(std::string other, int base): base must be beetween 2 and 62");
	if (other.empty()) {
		this->m_sign = false;
		this->m_number.push_back(false);
		return;
	}
	bool sign = false;
	if (other.front() == '-') {
		sign = true;
		other.assign(other, 1, other.size());
	} else if (other.front() == '+') {
		sign = false;
		other.assign(other, 1, other.size());
	}
	this->m_sign = false;
	this->m_number.push_back(false);
	InfInt p = InfInt::pos_one;
	InfInt infint_base = base;
	while (other.size() > 0) {
		unsigned char c = other.back();
		if ('0' <= c && c <= '9')
			c -= '0';
		else if ('a' <= c && c <= 'z')
			c -= 'a' - 10;
		else if ('A' <= c && c <= 'Z')
			c -= 'A' - 36;
		if (c < base)
			*this += InfInt(c) * p;
		other.pop_back();
		p *= infint_base;
	}
	this->clean();
	if (sign)
		this->twos_complement();
}

bool InfInt::sign(void) const {
	return this->m_sign;
}

bool InfInt::get(const size_type& pos) const {
	if (pos < this->size())
		return this->m_number[pos];
	else
		return this->m_sign;
}

typename InfInt::size_type InfInt::size(void) const {
	return this->m_number.size();
}

InfInt& InfInt::ones_complement(void) {
	this->m_sign = !sign();
	this->m_number.flip();
	return *this;
}

InfInt& InfInt::twos_complement(void) {
	if (*this == InfInt::zero)
		return *this;
	this->ones_complement();
	InfInt tmp;
	tmp.m_sign = this->sign();
	tmp.m_number.resize(this->size() + 1, false);
	tmp.m_number[1] = this->get(0);
	tmp.m_number[0] = !this->get(0);
	for (size_type i = 1; i < this->size(); ++i) {
		tmp.m_number[i + 1] = this->get(i) && tmp.get(i);
		tmp.m_number[i] = this->get(i) != tmp.get(i);
	}
	if (!tmp.m_number.back())
		tmp.m_number.pop_back();
	tmp.clean();
	*this = tmp;
	return *this;
}

std::string InfInt::Bstr(void) const {
	std::string str;
	str.reserve(this->size());
	if (this->m_sign)
		str.push_back('-');
	else
		str.push_back('+');
	for (size_t i = size() - 1; i > 0; --i) {
		str.push_back('0' + this->get(i));
	}
	if (this->size() > 0) {
		str.push_back('0' + this->get(0));
	}
	return str;
}

std::string InfInt::Bstr(size_type str_size) const {
	if (str_size == 0)
		str_size = this->size() + 1;
	if (str_size <= this->size())
		throw std::logic_error("std::string InfInt::b_str(size_type str_size) const: str_size <= size()");
	std::string str;
	str.reserve(str_size);
	for (; str_size > this->size(); --str_size)
		str.push_back('0' + this->m_sign);
	for (size_t i = this->size() - 1; i > 0; --i) {
		str.push_back('0' + get(i));
	}
	if (this->size() > 0) {
		str.push_back('0' + get(0));
	}
	return str;
}

std::string InfInt::str(int base) const {
	if (base < 2 || base > 62)
		throw std::domain_error("std::string InfInt::str(int base) const: base must be beetween 2 and 62");
	std::string str;
	InfInt tmp(*this);
	if (tmp == InfInt::zero)
		str.push_back('0');
	if (tmp.sign())
		tmp.twos_complement();
	InfInt remainder;
	InfInt infint_base(base);
	while (tmp != InfInt::zero) {
		auto result = InfInt::fulldiv(tmp, infint_base);
		tmp = result.quotient();
		remainder = result.remainder();
		if (remainder < 10)
			str.push_back('0' + remainder.to_int<char>());
		else if (remainder < 36)
			str.push_back('a' + remainder.to_int<char>() - 10);
		else
			str.push_back('A' + remainder.to_int<char>() - 36);
	}
	if (this->sign())
		str.push_back('-');
	std::reverse(str.begin(), str.end());
	return str;
}

std::string InfInt::str(void) const {
	InfInt powten = 10000000000000000000ull;
	InfInt::size_type size = 19;
	std::string str;
	InfInt tmp(*this);
	if (tmp.sign())
		tmp.twos_complement();
	while (true) { // do {...} while (temp != InfInt::zero);
		auto result = InfInt::fulldiv(tmp, powten);
		tmp = result.quotient();
		if (tmp == InfInt::zero) {
			str = std::to_string(result.remainder().to_int<unsigned long long>()) + str;
			break;
		}
		std::string tmpstr = std::to_string(result.remainder().to_int<unsigned long long>());
		InfInt::size_type tmpsize = size - tmpstr.size();
		str = tmpstr + str;
		tmpstr.clear();
		for (InfInt::size_type i = 0; i < tmpsize; ++i)
			tmpstr.push_back('0');
		str = tmpstr + str;
	}
	if (this->sign())
		return '-' + str;
	else
		return str;
}

InfIntFullDivResult InfInt::fulldiv(const InfInt& _a, const InfInt& _b) {
	if (_b == InfInt::zero)
		throw std::domain_error("static InfIntFulldivResult InfInt::fulldiv(const InfInt& _a, const InfInt& _b): Cannot divide by 0");

	InfInt q;
	InfInt a(_a);
	if (a.sign())
		a.twos_complement();
	InfInt abs_b(_b);
	if (abs_b.sign())
		abs_b.twos_complement();
	InfInt b(abs_b);

	if (b >= a) {
		InfInt remainder, quotient;
		if (b == a) {
			if (_a.sign() == _b.sign())
				quotient = InfInt::pos_one;
			else
				quotient = InfInt::neg_one;
		} else {
			if (a.sign() != _a.sign())
				a.twos_complement();
			remainder = a;
		}
		return InfIntFullDivResult(quotient, remainder);
	}

	InfInt::size_type size_diff = a.size() - b.size();
	InfInt mask = InfInt::pos_one << size_diff;
	b <<= size_diff;
	while (a >= abs_b) {
		while (a < b) {
			mask >>= 1;
			b >>= 1;
		}
		a -= b;
		q |= mask;
		mask >>= b.size() - a.size();
		b >>= b.size() - a.size();
	}

	InfInt quotient = q;
	InfInt remainder = a;
	if (_a.sign() != _b.sign())
		quotient.twos_complement();
	if (remainder.sign() != _a.sign())
		remainder.twos_complement();
	return InfIntFullDivResult(quotient, remainder);
}

InfInt& InfInt::operator=(const InfInt& other) {
	this->m_sign = other.sign();
	this->m_number = other.m_number;
	return *this;
}

bool InfInt::operator==(const InfInt& other) const {
	if (this->size() != other.size())
		return false;
	if (this->sign() != other.sign())
		return false;
	for (unsigned long long i = 0; i < this->size(); ++i)
		if (this->m_number[i] != other.get(i))
			return false;
	return true;
}

bool InfInt::operator!=(const InfInt& other) const {
	return !(*this == other);
}

bool InfInt::operator>(const InfInt& other) const {
	if (!this->sign() && other.sign())
		return true;
	if (this->sign() && !other.sign())
		return false;
	if (this->size() > other.size())
		return !this->sign();
	if (this->size() < other.size())
		return this->sign();
	if (this->size() > 1) {
		for (size_type i = this->size() - 2; i > 0; --i) {
			if (this->get(i) == other.get(i))
				continue;
			return this->get(i);
		}
	}
	if (this->size() != 0)
		return this->get(0) != other.get(0) && this->get(0);
	return false;
}

bool InfInt::operator<=(const InfInt& other) const {
	return !(*this > other);
}

bool InfInt::operator<(const InfInt& other) const {
	if (this->sign() && !other.sign())
		return true;
	if (!this->sign() && other.sign())
		return false;
	if (this->size() < other.size())
		return !this->sign();
	if (this->size() > other.size())
		return this->sign();
	if (this->size() > 1) {
		for (size_type i = this->size() - 2; i > 0; --i) {
			if (this->get(i) == other.get(i))
				continue;
			return other.get(i);
		}
	}
	if (this->size() != 0)
		return this->get(0) != other.get(0) && other.get(0);
	return false;
}

bool InfInt::operator>=(const InfInt& other) const {
	return !(*this < other);
}

InfInt InfInt::operator+(void) const {
	return *this;
}

InfInt InfInt::operator-(void) const {
	InfInt tmp(*this);
	tmp.twos_complement();
	return tmp;
}

InfInt InfInt::operator+(const InfInt& other) const {
	return InfInt(*this) += other;
}

InfInt& InfInt::operator+=(const InfInt& other) {
	size_type max_size;
	if (this->m_number.size() < other.m_number.size())
		max_size = other.size();
	else
		max_size = this->size();
	this->m_number.resize(max_size, this->sign());
	bool old_carry = false;
	for (size_type i = 0; i < max_size; ++i) {
		bool new_carry = ((this->get(i) != other.get(i)) && old_carry) || (this->get(i) && other.get(i));
		this->m_number[i] = (this->get(i) != other.get(i)) != old_carry;
		old_carry = new_carry;
	}
	if (this->sign() == other.sign())
		this->m_number.push_back(old_carry);
	else
		this->m_sign = !old_carry;
	return this->clean();
}

InfInt& InfInt::operator++(void) {
	*this += InfInt::pos_one;
	return *this;
}

InfInt InfInt::operator++(int) {
	InfInt tmp(*this);
	++(*this);
	return tmp;
}

InfInt InfInt::operator-(const InfInt& other) const {
	return InfInt(*this) -= other;
}

InfInt& InfInt::operator-=(const InfInt& other) {
	return (*this) += (-other);
}

InfInt& InfInt::operator--(void) {
	return *this += InfInt::neg_one;
}

InfInt InfInt::operator--(int) {
	InfInt tmp(*this);
	--(*this);
	return tmp;
}

InfInt InfInt::operator*(const InfInt& other) const {
	if (other.size() < this->size())
		return other * (*this);
	InfInt temp;
	InfInt a(*this);
	if (a.sign())
		a.twos_complement();
	InfInt b(other);
	if (b.sign())
		b.twos_complement();
	for (size_type i = 0; i < a.size(); ++i)
		if (a.get(i))
			temp += b << i;
	if (this->sign() != other.sign())
		temp.twos_complement();
	return temp;
}

InfInt& InfInt::operator*=(const InfInt& other) {
	*this = (*this) * other;
	return *this;
}

InfInt InfInt::operator/(const InfInt& other) const {
	if (other == InfInt::zero)
		throw std::domain_error("InfInt InfInt::operator/(const InfInt& other) const: Cannot divide by 0");

	InfInt q;
	InfInt a(*this);
	if (a.sign())
		a.twos_complement();
	InfInt abs_other(other);
	if (abs_other.sign())
		abs_other.twos_complement();
	InfInt b = abs_other;

	if (b >= a) {
		if (b == a) {
			if (this->sign() == other.sign())
				return InfInt::pos_one;
			else
				return InfInt::neg_one;
		} else
			return InfInt::zero;
	}

	size_type size_diff = a.size() - b.size();
	InfInt mask = InfInt::pos_one << size_diff;
	b <<= size_diff;
	while (a >= abs_other) {
		while (a < b) {
			mask >>= 1;
			b >>= 1;
		}
		a -= b;
		q |= mask;
		mask >>= b.size() - a.size();
		b >>= b.size() - a.size();
	}

	if (this->sign() != other.sign())
		q.twos_complement();
	return q;
}

InfInt& InfInt::operator/=(const InfInt& other) {
	*this = *this / other;
	return *this;
}

InfInt InfInt::operator%(const InfInt& other) const {
	if (other == InfInt::zero)
		throw std::domain_error("InfInt InfInt::operator%(const InfInt& other) const: Cannot divide by 0");

	InfInt a(*this);
	if (a.sign())
		a.twos_complement();
	InfInt abs_other(other);
	if (abs_other.sign())
		abs_other.twos_complement();
	InfInt b = abs_other;

	if (b >= a) {
		if (b == a)
			return InfInt::zero;
		else {
			if (a.sign() != this->sign())
				a.twos_complement();
			return a;
		}
	}

	size_type size_diff = a.size() - b.size();
	b <<= size_diff;
	while (a >= abs_other) {
		while (a < b)
			b >>= 1;
		a -= b;
		b >>= b.size() - a.size();
	}

	if (a.sign() != this->sign())
		a.twos_complement();
	return a;
}

InfInt& InfInt::operator%=(const InfInt& other) {
	*this = *this % other;
	return *this;
}

InfInt InfInt::operator&(const InfInt& other) const {
	size_type max_size = 1;
	if (this->size() < other.size())
		max_size = other.size();
	else
		max_size = this->size();
	InfInt temp;
	temp.m_sign = this->sign() && other.sign();
	temp.m_number.resize(max_size, temp.sign());
	for (size_type i = 0; i < max_size; ++i)
		temp.m_number[i] = this->get(i) && other.get(i);
	return temp.clean();
}

InfInt& InfInt::operator&=(const InfInt& other) {
	size_type max_size = 1;
	if (this->size() < other.size())
		max_size = other.size();
	else
		max_size = this->size();
	this->m_sign = this->sign() && other.sign();
	this->m_number.resize(max_size, this->sign());
	for (size_type i = 0; i < max_size; ++i)
		this->m_number[i] = this->get(i) && other.get(i);
	return this->clean();
}

InfInt InfInt::operator|(const InfInt& other) const {
	size_type max_size;
	if (this->size() < other.size())
		max_size = other.size();
	else
		max_size = this->size();
	InfInt temp;
	temp.m_sign = this->sign() || other.sign();
	temp.m_number.resize(max_size, temp.sign());
	for (size_type i = 0; i < max_size; ++i)
		temp.m_number[i] = this->get(i) || other.get(i);
	return temp.clean();
}

InfInt& InfInt::operator|=(const InfInt& other) {
	size_type max_size;
	if (this->size() < other.size())
		max_size = other.size();
	else
		max_size = this->size();
	this->m_sign = this->sign() || other.sign();
	this->m_number.resize(max_size, this->sign());
	for (size_type i = 0; i < max_size; ++i)
		this->m_number[i] = this->get(i) || other.get(i);
	return this->clean();
}

InfInt InfInt::operator^(const InfInt& other) const {
	size_type max_size;
	if (this->size() < other.size())
		max_size = other.size();
	else
		max_size = this->size();
	InfInt temp;
	temp.m_sign = this->sign() != other.sign();
	temp.m_number.resize(max_size, temp.sign());
	for (size_type i = 0; i < max_size; ++i)
		temp.m_number[i] = this->get(i) != other.get(i);
	return temp.clean();
}

InfInt& InfInt::operator^=(const InfInt& other) {
	size_type max_size;
	if (this->size() < other.size())
		max_size = other.size();
	else
		max_size = this->size();
	this->m_sign = this->sign() != other.sign();
	this->m_number.resize(max_size, this->sign());
	for (size_type i = 0; i < max_size; ++i)
		this->m_number[i] = this->get(i) != other.get(i);
	return this->clean();
}

InfInt InfInt::operator<<(size_type other) const {
	if (other == 0 || (*this) == InfInt::zero)
		return *this;
	InfInt temp(*this);
	temp.m_number.insert(temp.m_number.begin(), other, false);
	return temp;
}

InfInt& InfInt::operator<<=(size_type other) {
	if (other == 0 || (*this) == InfInt::zero)
		return *this;
	this->m_number.insert(this->m_number.begin(), other, false);
	return *this;
}

InfInt InfInt::operator>>(size_type other) const {
	if (other == 0)
		return *this;
	InfInt temp(*this);
	if (other < size())
		temp.m_number.erase(temp.m_number.begin(), temp.m_number.begin() + static_cast<long long>(other));
	else
		temp.m_number.clear();
	temp.clean();
	return temp;
}

InfInt& InfInt::operator>>=(size_type other) {
	if (other == 0)
		return *this;
	if (other < this->size())
		this->m_number.erase(this->m_number.begin(), this->m_number.begin() + static_cast<long long>(other));
	else
		this->m_number.clear();
	this->clean();
	return *this;
}

InfInt& InfInt::clean(void) {
	while (this->size() > 1 && this->m_number.back() == this->m_sign)
		this->m_number.pop_back();
	if (this->size() == 0)
		this->m_number.push_back(this->m_sign);
	return *this;
}

const InfInt InfInt::neg_one(-1);
const InfInt InfInt::zero(0);
const InfInt InfInt::pos_one(1);



// Outside the class //

// alows the use of the macro _inf to transform a unsigned long long to an InfInt
InfInt operator "" _infint(unsigned long long other) {
	return static_cast<InfInt>(other);
}

std::ostream& operator<<(std::ostream& out, const InfInt& infint) {
	out << infint.str();
	return out;
}

std::istream& operator>>(std::istream& in, InfInt& infint) {
	std::string str;
	in >> str;
	infint = InfInt(str, 10);
	return in;
}

#endif // INFINT_HPP