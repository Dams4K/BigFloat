#include "register_types.h"
#include "core/object/class_db.h"
#include "modules/bigfloat/big_float.h"

void initialize_bigfloat_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	ClassDB::register_class<BigFloat>();
}

void uninitialize_bigfloat_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}
