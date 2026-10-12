#pragma once

#include "core/string/ustring.h"
namespace bigfloat_backend {

struct DecimalParts {
	bool neg = false;
	String digits;
	int64_t exp10 = 0;
};

enum RoundMode {
	TRUNCATE,
	ROUND,
};

}
