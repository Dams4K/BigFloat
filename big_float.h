#pragma once

#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/variant/dictionary.h"
#include "core/variant/variant.h"

#include "backend/backend.h"

class BigFloat : public RefCounted {
	GDCLASS(BigFloat, RefCounted)

	BigFloatImpl value;
	static bool _variant_to_impl(const Variant& p_val, BigFloatImpl& r_out);

protected:
	static void _bind_methods();

public:
	static Ref<BigFloat> from_int(int64_t p_value);
	static Ref<BigFloat> from_float(double p_value);

	bool is_less_than(const Variant& p_other) const;
	bool is_greater_than(const Variant& p_other) const;
	bool is_less_than_or_equal_to(const Variant& p_other) const;
	bool is_greater_than_or_equal_to(const Variant& p_other) const;
	bool is_equal_to(const Variant& p_other) const;
	bool is_different_to(const Variant& p_other) const;

	Ref<BigFloat> add_assign(const Variant& p_val);
	Ref<BigFloat> sub_assign(const Variant& p_val);
	Ref<BigFloat> mul_assign(const Variant& p_val);
	Ref<BigFloat> div_assign(const Variant& p_val);

	Ref<BigFloat> add(const Variant& p_val) const;
	Ref<BigFloat> sub(const Variant& p_val) const;
	Ref<BigFloat> mul(const Variant& p_val) const;
	Ref<BigFloat> div(const Variant& p_val) const;

	String _to_string() override;
	String to_plain_scientific() const;
	String to_scientific() const;
	String to_aa() const;
	String to_metric_name() const;
	String to_metric_symbol() const;
	String to_short_scale() const;
};

class BigNumber : public RefCounted {
	GDCLASS(BigNumber, RefCounted)

public:
	// Constants
	static const double MANTISSA_MAX;
	static const double MANTISSA_PRECISION;

	BigNumber();
	BigNumber(const String &p_string);
	BigNumber(int64_t p_int);
	BigNumber(double p_float);
	BigNumber(double p_mantissa, int64_t p_exponent);
	BigNumber(const Variant &p_val);
	~BigNumber();

	void set_mantissa(double p_mantissa);
	double get_mantissa() const;

	void set_exponent(int64_t p_exponent);
	int64_t get_exponent() const;

	void normalize();

	bool is_less_than(const Variant &n) const;
	bool is_equal_to(const Variant &n) const;
	bool is_greater_than(const Variant &n) const;
	bool is_less_than_or_equal_to(const Variant &n) const;
	bool is_greater_than_or_equal_to(const Variant &n) const;

	Ref<BigNumber> plus(const Variant &n) const;
	Ref<BigNumber> plus_equals(const Variant &n);
	Ref<BigNumber> minus(const Variant &n) const;
	Ref<BigNumber> minus_equals(const Variant &n);
	Ref<BigNumber> multiply(const Variant &n) const;
	Ref<BigNumber> multiply_equals(const Variant &n);
	Ref<BigNumber> divide(const Variant &n) const;
	Ref<BigNumber> divide_equals(const Variant &n);

	Ref<BigNumber> mod(const Variant &n) const;
	Ref<BigNumber> power(const Variant &n) const;
	Ref<BigNumber> power_equals(const Variant &n);
	Ref<BigNumber> square_root() const;
	Ref<BigNumber> absolute() const;

	double log10() const;
	double ln() const;
	void floor_value();
	double to_float() const;
	String to_plain_scientific() const;

	String _to_string() override;

	// Formatting methods
	String to_scientific(bool no_decimals_on_small_values = false, bool force_decimals = false) const;
	String to_prefix(bool no_decimals_on_small_values = false, bool use_thousand_symbol = true, bool force_decimals = true, bool scientific_prefix = false) const;
	String to_aa(bool no_decimals_on_small_values = false, bool use_thousand_symbol = true, bool force_decimals = false) const;
	String to_metric_symbol(bool no_decimals_on_small_values = false) const;
	String to_metric_name(bool no_decimals_on_small_values = false) const;
	String to_short_scale(bool no_decimals_on_small_values = false) const;

	// Static configuration
	static Dictionary get_options();

protected:
	static void _bind_methods();

private:
	static Ref<BigNumber> _type_check(const Variant &n);
	static void _size_check(double p_mantissa);
	static void _get_values(const Variant &n, double &r_mantissa, int64_t &r_exponent);

	double mantissa = 1.0;
	int64_t exponent = 0;
};
