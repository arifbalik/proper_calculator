#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define MAX_QUERY_LENGTH 80
#include "../parser/symbol_table.h"

/* Error defs. */
typedef enum {
	NONE = 255,

	/* Those are system spesific events.
	 * Usually lexer uses them.
	 */
	_PDONE, /* Parse Done */
	_PINP, /* Parse In Proggress */
	_PERR, /* Parse Error */
	_APQ, /* Append To New Query */

	_FEX, /* Function Expand */

	/* Euler std. errors */
	MTHE = -128, /* Math Error */
	EMPTYQ, /* Empty Query */
	SYMNF, /* Symbol Not Found */

	/* Variable errors */
	MXVQ, /* Max Variable Quantity Has Been Reaced */
	NVF, /* No Variable Found */

	/* Function errors */
	FNF, /* Function Not Found */
	CNEF, /* Can Not Evaluate Function */
	UNDEFR /* Undefined Eesult Type */
} estatus_t;

typedef enum { NO_RESULT = -128, FN_EVAL, INTEGER, FRACTION, MATRIX } etype_t;

/* Results will be stored here by the parser */
typedef union {
	double fraction;
	double **matrix;
	char *var;
} numerical_t;

typedef struct {
	etype_t type; /* Developer should check this type before any action */
	numerical_t resultn; /* Numerical results will be stored here */
	estatus_t status; /* Developer can use this to debug the engine */

} ersl_t;

void parse_query(char *query, ersl_t *ersl);
