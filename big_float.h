#pragma once

#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/variant/dictionary.h"
#include "core/variant/type_info.h"
#include "core/variant/variant.h"

#include "backend/big_float_impl.h"
#include "backend/backend.h"

using bigfloat_backend::RoundMode;
using bigfloat_backend::DecimalParts;

class BigFloat : public RefCounted {
	GDCLASS(BigFloat, RefCounted)

	BigFloatImpl value;
	static bool _variant_to_impl(const Variant& p_val, BigFloatImpl& r_out);

protected:
	static void _bind_methods();

public:
	static Ref<BigFloat> from_int(int64_t p_value);
	static Ref<BigFloat> from_float(double p_value);

	bool is_less_than(const Variant& p_val) const;
	bool is_greater_than(const Variant& p_val) const;
	bool is_less_than_or_equal_to(const Variant& p_val) const;
	bool is_greater_than_or_equal_to(const Variant& p_val) const;
	bool is_equal_to(const Variant& p_val) const;
	bool is_different_to(const Variant& p_val) const;

	Ref<BigFloat> add_assign(const Variant& p_val);
	Ref<BigFloat> sub_assign(const Variant& p_val);
	Ref<BigFloat> mul_assign(const Variant& p_val);
	Ref<BigFloat> div_assign(const Variant& p_val);

	Ref<BigFloat> add(const Variant& p_val) const;
	Ref<BigFloat> sub(const Variant& p_val) const;
	Ref<BigFloat> mul(const Variant& p_val) const;
	Ref<BigFloat> div(const Variant& p_val) const;

	Ref<BigFloat> mod(const Variant& p_val) const;
	Ref<BigFloat> power(const Variant& p_val) const;
	Ref<BigFloat> sqrt() const;
	Ref<BigFloat> absolute() const;

	Ref<BigFloat> floor() const;
	Ref<BigFloat> ceil() const;

	Ref<BigFloat> mod_assign(const Variant& p_val);
	Ref<BigFloat> power_assign(const Variant& p_val);
	Ref<BigFloat> sqrt_assign();
	Ref<BigFloat> absolute_assign();

	Ref<BigFloat> floor_assign();
	Ref<BigFloat> ceil_assign();

	String _to_string() override;
	String to_plain_string(int max_digits = 1000, RoundMode mode = RoundMode::ROUND) const;

	double to_float() const;
	String to_scientific() const;
	String to_aa() const;
	String to_metric_name() const;
	String to_metric_symbol() const;
	String to_short_scale() const;
};

VARIANT_ENUM_CAST(RoundMode);
