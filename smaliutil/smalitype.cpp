#include <stdlib.h>
#include <string.h>

#include "smalitype.h"

namespace smali {

/**
 * code_t constructor.
 */
code_t::code_t() {
	blockflags = 0;
	blockid = 0;
}

/**
 * method_t constructor.
 */
method_t::method_t() {
	flags = 0;
	locals = 0;
	start = 0;
	end = 0;
}

/**
 * field_t constructor.
 */
field_t::field_t() {
	line = 0;
}

/**
 * class_t constructor.
 */
class_t::class_t() {
}

/**
 * metadata_t constructor.
 */
metadata_t::metadata_t() {
	memset(&raw, 0, sizeof(raw));
}

}	/* namespace smali */

