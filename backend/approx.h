#pragma once

#include "core/string/ustring.h"
#include <cstdint>

namespace bigfloat_backend {

constexpr double LOG_10 = 2.302585092994046;

struct Approx {
	// A double preserves ~15.95 decimal digits (53 bits of significand).
	// Beyond this exponent difference, the smaller operand is absorbed
	// by rounding and no longer affects the sum.
	static constexpr int64_t ABSORB_DIGITS = 17;

	double mantissa = 0.0;
	int64_t exponent = 0;

	static Approx from_string(const String& p_str);
	static Approx from_int(int64_t p_value);
	static Approx from_double(double p_value);
	static Approx from_parts(double p_mantissa, int64_t p_exponent);

	void normalize();
	int cmp(const Approx& p_other) const;
	bool is_zero() const {
		return mantissa == 0.0;
	}

	inline Approx operator-() const {
		Approx r = *this;
		r.mantissa = -r.mantissa;
		return r;
	};

	Approx& operator+=(const Approx& p_other);
	inline Approx& operator-=(const Approx& p_other) {
		return *this += -p_other;
	};
	Approx& operator*=(const Approx& p_other);
	Approx& operator/=(const Approx& p_other);

	String to_string() const;
	String to_plain_scientific() const {
		return String::num(mantissa) + "e" + String::num_int64(exponent);
	}
	String to_scientific() const;
	String as_prefix() const;
};


inline Approx operator+(Approx p_a, const Approx& p_b) {
	p_a += p_b;
	return p_a;
}

inline Approx operator-(Approx p_a, const Approx& p_b) {
	p_a -= p_b;
	return p_a;
}

inline Approx operator*(Approx p_a, const Approx& p_b) {
	p_a *= p_b;
	return p_a;
}

inline Approx operator/(Approx p_a, const Approx& p_b) {
	p_a /= p_b;
	return p_a;
}

inline bool operator<(const Approx& p_a, const Approx& p_b) {
	return p_a.cmp(p_b) < 0;
}

inline bool operator<=(const Approx& p_a, const Approx& p_b) {
	return p_a.cmp(p_b) <= 0;
}

inline bool operator >(const Approx& p_a, const Approx& p_b) {
	return p_a.cmp(p_b) > 0;
}

inline bool operator>=(const Approx& p_a, const Approx& p_b) {
	return p_a.cmp(p_b) >= 0;
}

inline bool operator==(const Approx& p_a, const Approx& p_b) {
	return p_a.mantissa == p_b.mantissa && p_a.exponent == p_b.mantissa;
}

inline bool operator!=(const Approx& p_a, const Approx& p_b) {
	return !(p_a == p_b);
}

}
