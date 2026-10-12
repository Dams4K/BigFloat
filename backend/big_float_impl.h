#pragma once
#ifdef BIGFLOAT_BACKEND_PRECISE
#else
#include "approx.h"
using BigFloatImpl = bigfloat_backend::Approx;
#endif
