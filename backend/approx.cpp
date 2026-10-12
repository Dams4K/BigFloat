#include "approx.h"
#include "core/math/math_funcs.h"
#include "core/typedefs.h"
#include "modules/bigfloat/backend/backend.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>

namespace bigfloat_backend {

Approx Approx::from_string(const String &p_str) {
	return Approx();
}

Approx Approx::from_int(int64_t p_value) {
	return from_double(p_value);
}

Approx Approx::from_double(double p_value) {
	return from_parts(p_value, 0);
}

Approx Approx::from_parts(double p_mantissa, int64_t p_exponent) {
	Approx a;
	a.mantissa = p_mantissa;
	a.exponent = p_exponent;
	a.normalize();
	return a;
}

void Approx::normalize() {
	if (is_zero()) {
		exponent = 0;
		return;
	}

	double abs_m = Math::abs(mantissa);
	if (abs_m >= 1.0 && abs_m < 10.0) {
		return;
	}

	int64_t shift = (int64_t) Math::floor(Math::log(abs_m) / LOG_10);
	mantissa /= Math::pow(10.0, (double) shift);
	exponent += shift;

	// log(1000) / LOG_10 return 2.9999999999999996 instead of 3
	abs_m = Math::abs(mantissa);
	if (abs_m >= 10) {
		mantissa /= 10;
		exponent += 1;
	} else if (abs_m < 1.0) {
		mantissa *= 10;
		exponent -= 1;
	}
}

int Approx::cmp(const Approx &p_other) const {
	int sign_a = (mantissa > 0.0) - (mantissa < 0.0);
	int sign_b = (p_other.mantissa > 0.0) - (p_other.mantissa < 0.0);

	if (sign_a != sign_b) {
		return sign_a < sign_b ? -1 : 1;
	}

	if (sign_a == 0) {
		return 0;
	}

	if (exponent != p_other.exponent) {
		int result = exponent < p_other.exponent ? -1 : 1;
		return sign_a > 0 ? result : -result;
	}

	if (mantissa < p_other.mantissa) {
		return -1;
	}

	return mantissa > p_other.mantissa ? 1 : 0;
}


Approx& Approx::operator+=(const Approx& p_other) {
	if (p_other.is_zero()) {
		return *this;
	}
	if (is_zero()) {
		*this = p_other;
		return *this;
	}

	int64_t diff = p_other.exponent - exponent;
	if (diff > MAX_DIGITS) {
		*this = p_other;
		return *this;
	} else if (diff < -MAX_DIGITS) {
		return *this;
	}

	if (diff >= 0) {
		mantissa += p_other.mantissa * Math::pow(10.0, (double) diff);
	} else {
		mantissa += p_other.mantissa / Math::pow(10.0, (double) -diff);
	}

	normalize();
	return *this;
}

Approx& Approx::operator*=(const Approx &p_other) {
	if (is_zero() || p_other.is_zero()) {
		mantissa = 0.0;
		exponent = 0;
		return *this;
	}

	exponent += p_other.exponent;
	mantissa *= p_other.mantissa;

	// |mantissa| is now in [1, 100): no need for a log and a pow
	if (Math::abs(mantissa) >= 10.0) {
		mantissa /= 10.0;
		exponent += 1;
	}

	return *this;
}

Approx& Approx::operator/=(const Approx &p_other) {
	if (p_other.is_zero()) { // error handled by big_float
		return *this;
	}
	if (is_zero()) {
		return *this;
	}

	mantissa /= p_other.mantissa;
	exponent -= p_other.exponent;

	// (0.1, 10.0)
	if (Math::abs(mantissa) < 1.0) {
		mantissa *= 10.0;
		exponent -= 1;
	}

	return *this;
}

double Approx::to_double() const {
	return 0.0;
}

DecimalParts Approx::to_decimal_parts(int p_precision, RoundMode p_round_mode) const {
	DecimalParts p;
	if (is_zero()) {
		p.digits = "0";
		p.exp10 = 0;
		return p;
	}

	p.neg = mantissa < 0.0;
	double m = Math::abs(mantissa);

	p_precision = CLAMP(p_precision, 1, MAX_DIGITS);

	char buf[80];
	if (p_round_mode == TRUNCATE) {
		snprintf(buf, sizeof(buf), "%.60e", m);
	} else {
		snprintf(buf, sizeof(buf), "%.*e", p_precision-1, m);
	}

	String digits;

	int i = 0;
	for (; buf[i] != '\0' && buf[i] != 'e'; ++i) {
		if (buf[i] != '.') {
			digits += buf[i];
		}
	}

	if (digits.length() > p_precision) {
		digits = digits.substr(0, p_precision);
	}

	// In round mode, 9.99 => 1.00+e01, so we add 1 to the exponent
	int64_t e_printf = atoll(buf + i + 1);
	p.exp10 = exponent + e_printf;

	int end = digits.length();
	while (end > 1 && digits[end-1] == U'0') {
		--end;
	}
	p.digits = digits.substr(0, end);

	return p;
}

}
