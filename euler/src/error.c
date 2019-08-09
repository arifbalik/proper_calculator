#include "../inc/euler.h"

char *serr(int8_t err)
{
	switch (err) {
	case MXVQ:
		return strdup("Max variable length has been reached!");
	case NVF:
		return strdup("Variable Not found!");
	case FNF:
		return strdup("Function Not found!");
	case CNEF:
		return strdup("Cannot evaluate function!");
	case UNDEFR:
		return strdup("Undefined result type!");
	default:
		return strdup("Unkown error!");
	}
}
