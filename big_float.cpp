#include "big_float.h"
#include "core/error/error_macros.h"
#include "core/math/math_funcs.h"
#include "core/object/class_db.h"
#include "core/object/ref_counted.h"
#include "modules/bigfloat/backend/big_float_impl.h"

const char *METRIC_SYMBOLS[] = {
	"", "k", "M", "G", "T", "P", "E", "Z", "Y", "R", "Q",
	"V", "U", "Td", "S", "Ri", "Qx", "Pp", "O", "N", "Mi"
};

const char *METRIC_NAMES[] = {
	"", "kilo", "mega", "giga", "tera", "peta", "exa", "zetta", "yotta", "ronna", "quetta",
	"vunda", "uda", "treda", "sorta", "rinta", "quexa", "pepta", "ocha", "nena", "ming"
};

const char *SHORT_SCALE_NAMES[] = {
	"", "thousand", "million", "billion", "trillion", "quadrillion", "quintillion", "sextillion", "septillion", "octillion", "nonillion",
	"decillion", "undecillion", "duodecillion", "tredecillion", "quattuordecillion", "quindecillion", "sexdecillion", "septendecillion", "octodecillion", "novemdecillion"
};

const char *ALPHABET[] = {
	"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
	"n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"
};

bool BigFloat::_variant_to_impl(const Variant &p_val, BigFloatImpl &r_out) {
	switch (p_val.get_type()) {
		case Variant::INT:
			r_out = BigFloatImpl::from_int(p_val);
			return true;
		case Variant::FLOAT: {
			double d = p_val;
			ERR_FAIL_COND_V_MSG(!Math::is_finite(d), false, "BigFloat: NaN and infinity are not yet supported");
			r_out = BigFloatImpl::from_double(d);
			return true;
		}
		case Variant::STRING: {
			r_out = BigFloatImpl::from_string(p_val);
			return true;
		}
		case Variant::OBJECT: {
			Ref<BigFloat> b = p_val;
			ERR_FAIL_COND_V_MSG(b.is_null(), false, "BigNumber: invalid object");
			r_out = b->value;
			return true;
		}
		default:
			ERR_FAIL_V_MSG(false, "BigFloat: unsupported type");
	}

	return false;
}

Ref<BigFloat> BigFloat::from_int(int64_t p_value) {
	Ref<BigFloat> b;
	b.instantiate();
	b->value = BigFloatImpl::from_int(p_value);
	return b;
}

Ref<BigFloat> BigFloat::from_float(double p_value) {
	Ref<BigFloat> b;
	b.instantiate();
	b->value = BigFloatImpl::from_double(p_value);
	return b;
}

bool BigFloat::is_less_than(const Variant &p_val) const {
	BigFloatImpl other;
	ERR_FAIL_COND_V(!_variant_to_impl(p_val, other), false);
	return value < other;
}

bool BigFloat::is_greater_than(const Variant &p_val) const {
	BigFloatImpl other;
	ERR_FAIL_COND_V(!_variant_to_impl(p_val, other), false);
	return value > other;
}

bool BigFloat::is_less_than_or_equal_to(const Variant &p_val) const {
	BigFloatImpl other;
	ERR_FAIL_COND_V(!_variant_to_impl(p_val, other), false);
	return value <= other;
}

bool BigFloat::is_greater_than_or_equal_to(const Variant &p_val) const {
	BigFloatImpl other;
	ERR_FAIL_COND_V(!_variant_to_impl(p_val, other), false);
	return value >= other;
}

bool BigFloat::is_equal_to(const Variant &p_val) const {
	BigFloatImpl other;
	ERR_FAIL_COND_V(!_variant_to_impl(p_val, other), false);
	return value == other;
}

bool BigFloat::is_different_to(const Variant &p_val) const {
	BigFloatImpl other;
	ERR_FAIL_COND_V(!_variant_to_impl(p_val, other), false);
	return value != other;
}

Ref<BigFloat> BigFloat::add_assign(const Variant& p_val) {
	BigFloatImpl other;
	ERR_FAIL_COND_V(!_variant_to_impl(p_val, other), Ref<BigFloat>(this));
	value += other;
	return Ref<BigFloat>(this);
}

Ref<BigFloat> BigFloat::sub_assign(const Variant &p_val) {
	BigFloatImpl other;
	ERR_FAIL_COND_V(!_variant_to_impl(p_val, other), Ref<BigFloat>(this));
	value -= other;
	return Ref<BigFloat>(this);
}

Ref<BigFloat> BigFloat::mul_assign(const Variant &p_val) {
	BigFloatImpl other;
	ERR_FAIL_COND_V(!_variant_to_impl(p_val, other), Ref<BigFloat>(this));
	value *= other;
	return Ref<BigFloat>(this);
}

Ref<BigFloat> BigFloat::div_assign(const Variant &p_val) {
	BigFloatImpl other;
	ERR_FAIL_COND_V(!_variant_to_impl(p_val, other), Ref<BigFloat>(this));
	value /= other;
	return Ref<BigFloat>(this);
}

Ref<BigFloat> BigFloat::add(const Variant &p_val) const {
	BigFloatImpl other;
	ERR_FAIL_COND_V(!_variant_to_impl(p_val, other), Ref<BigFloat>());

	Ref<BigFloat> result;
	result.instantiate();
	result->value = value + other;
	return result;
}

Ref<BigFloat> BigFloat::sub(const Variant &p_val) const {
	BigFloatImpl other;
	ERR_FAIL_COND_V(!_variant_to_impl(p_val, other), Ref<BigFloat>());

	Ref<BigFloat> result;
	result.instantiate();
	result->value = value - other;
	return result;
}

Ref<BigFloat> BigFloat::mul(const Variant &p_val) const {
	BigFloatImpl other;
	ERR_FAIL_COND_V(!_variant_to_impl(p_val, other), Ref<BigFloat>());

	Ref<BigFloat> result;
	result.instantiate();
	result->value = value * other;
	return result;
}

Ref<BigFloat> BigFloat::div(const Variant &p_val) const {
	BigFloatImpl other;
	ERR_FAIL_COND_V(!_variant_to_impl(p_val, other), Ref<BigFloat>());

	Ref<BigFloat> result;
	result.instantiate();
	result->value = value / other;
	return result;
}

Ref<BigFloat> BigFloat::mod(const Variant &p_val) const {
	BigFloatImpl other;
	ERR_FAIL_COND_V(!_variant_to_impl(p_val, other), Ref<BigFloat>());

	Ref<BigFloat> result;
	result.instantiate();
	result->value = value % other;
	return result;
}

Ref<BigFloat> BigFloat::power(const Variant &p_val) const {
	BigFloatImpl other;
	ERR_FAIL_COND_V(!_variant_to_impl(p_val, other), Ref<BigFloat>());

	Ref<BigFloat> result;
	result.instantiate();
	result->value = value.pow(other);
	return result;
}

Ref<BigFloat> BigFloat::sqrt() const {
	Ref<BigFloat> result;
	result.instantiate();
	result->value = value;
	return result->sqrt_assign();
}

Ref<BigFloat> BigFloat::absolute() const {
	Ref<BigFloat> result;
	result.instantiate();
	result->value = value.absolute();
	return result;
}

Ref<BigFloat> BigFloat::floor() const {
	Ref<BigFloat> result;
	result.instantiate();
	result->value = value.floor();
	return result;
}

Ref<BigFloat> BigFloat::ceil() const {
	Ref<BigFloat> result;
	result.instantiate();
	result->value = value.ceil();
	return result;
}

Ref<BigFloat> BigFloat::mod_assign(const Variant &p_val) {
	BigFloatImpl other;
	ERR_FAIL_COND_V(!_variant_to_impl(p_val, other), Ref<BigFloat>(this));
	value %= other;
	return Ref<BigFloat>(this);
}

Ref<BigFloat> BigFloat::power_assign(const Variant &p_val) {
	BigFloatImpl other;
	ERR_FAIL_COND_V(!_variant_to_impl(p_val, other), Ref<BigFloat>(this));
	value.pow_assign(other);
	return Ref<BigFloat>(this);
}

Ref<BigFloat> BigFloat::sqrt_assign() {
	// ERR_FAIL_COND_V_MSG(value < 0, Ref<BigFloat>(this), "Can't calculate the square root value of negative value.");
	value.sqrt_assign();
	return Ref<BigFloat>(this);
}

Ref<BigFloat> BigFloat::absolute_assign() {
	value.absolute_assign();
	return Ref<BigFloat>(this);
}

Ref<BigFloat> BigFloat::floor_assign() {
	value.floor_assign();
	return Ref<BigFloat>(this);
}

Ref<BigFloat> BigFloat::ceil_assign() {
	value.ceil_assign();
	return Ref<BigFloat>(this);
}


String BigFloat::_to_string() {
	return to_plain_string(15);
}

String BigFloat::to_plain_string(int max_digits, RoundMode mode) const {
	DecimalParts p = value.to_decimal_parts(15, mode);
	int point = p.exp10 + 1; // one number before the decimal part
	if (Math::abs(point) > max_digits) {
		// return to_plain_scientific();
	}

	String s;
	if (point <= 0) {
		s = "0." + String("0").repeat(-point) + p.digits;
	} else if (point >= p.digits.length()) {
		s = p.digits + String("0").repeat(point - p.digits.length());
	} else {
		s = p.digits.substr(0, point) + "." + p.digits.substr(point);
	}
	return (p.neg ? "-" : "") + s;
}

double BigFloat::to_float() const {
	return value.to_double();
}


String BigFloat::to_scientific() const {
	return String();
}

String BigFloat::to_aa() const {
	return String();
}

String BigFloat::to_metric_name() const {
	return String();
}

String BigFloat::to_metric_symbol() const {
	return String();
}

String BigFloat::to_short_scale() const {
	return String();
}

void BigFloat::_bind_methods() {
	ClassDB::bind_static_method("BigFloat", D_METHOD("from_int", "value"), &BigFloat::from_int);
	ClassDB::bind_static_method("BigFloat", D_METHOD("from_float", "value"), &BigFloat::from_float);

	ClassDB::bind_method(D_METHOD("add_assign", "value"), &BigFloat::add_assign);
	ClassDB::bind_method(D_METHOD("sub_assign", "value"), &BigFloat::sub_assign);
	ClassDB::bind_method(D_METHOD("mul_assign", "value"), &BigFloat::mul_assign);
	ClassDB::bind_method(D_METHOD("div_assign", "value"), &BigFloat::div_assign);

	ClassDB::bind_method(D_METHOD("add", "value"), &BigFloat::add);
	ClassDB::bind_method(D_METHOD("sub", "value"), &BigFloat::sub);
	ClassDB::bind_method(D_METHOD("mul", "value"), &BigFloat::mul);
	ClassDB::bind_method(D_METHOD("div", "value"), &BigFloat::div);

	ClassDB::bind_method(D_METHOD("is_less_than", "other"), &BigFloat::is_less_than);
	ClassDB::bind_method(D_METHOD("is_greater_than", "other"), &BigFloat::is_greater_than);
	ClassDB::bind_method(D_METHOD("is_less_than_or_equal_to", "other"), &BigFloat::is_less_than_or_equal_to);
	ClassDB::bind_method(D_METHOD("is_greater_than_or_equal_to", "other"), &BigFloat::is_greater_than_or_equal_to);
	ClassDB::bind_method(D_METHOD("is_equal_to", "other"), &BigFloat::is_equal_to);
	ClassDB::bind_method(D_METHOD("is_different_to", "other"), &BigFloat::is_different_to);

	ClassDB::bind_method(D_METHOD("mod", "other"), &BigFloat::mod);
	ClassDB::bind_method(D_METHOD("power", "other"), &BigFloat::power);
	ClassDB::bind_method(D_METHOD("sqrt"), &BigFloat::sqrt);
	ClassDB::bind_method(D_METHOD("absolute"), &BigFloat::absolute);

	ClassDB::bind_method(D_METHOD("floor"), &BigFloat::floor);
	ClassDB::bind_method(D_METHOD("ceil"), &BigFloat::ceil);

	ClassDB::bind_method(D_METHOD("mod_assign", "other"), &BigFloat::mod_assign);
	ClassDB::bind_method(D_METHOD("power_assign", "other"), &BigFloat::power_assign);
	ClassDB::bind_method(D_METHOD("sqrt_assign"), &BigFloat::sqrt_assign);
	ClassDB::bind_method(D_METHOD("absolute_assign"), &BigFloat::absolute_assign);

	ClassDB::bind_method(D_METHOD("floor_assign"), &BigFloat::floor_assign);
	ClassDB::bind_method(D_METHOD("ceil_assign"), &BigFloat::ceil_assign);

	ClassDB::bind_method(D_METHOD("to_plain_string", "max_digits", "mode"), &BigFloat::to_plain_string, DEFVAL(1000), DEFVAL(RoundMode::ROUND));

	ClassDB::bind_method(D_METHOD("to_scientific"), &BigFloat::to_scientific);
	ClassDB::bind_method(D_METHOD("to_aa"), &BigFloat::to_aa);
	ClassDB::bind_method(D_METHOD("to_metric_name"), &BigFloat::to_metric_name);
	ClassDB::bind_method(D_METHOD("to_metric_symbol"), &BigFloat::to_metric_symbol);
	ClassDB::bind_method(D_METHOD("to_short_scale"), &BigFloat::to_short_scale);

	BIND_ENUM_CONSTANT(RoundMode::TRUNCATE);
	BIND_ENUM_CONSTANT(RoundMode::ROUND);
}
