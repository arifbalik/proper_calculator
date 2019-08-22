#include <math.h>
#include <stdint.h>
#define MAX_QUERY_LENGTH 80
#include "../parser/symbol_table.h"
#include "_atof.h"
#include "_itoa.h"
#include "_strcpy.h"
#include "_strlen.h"
#include "_strstr.h"
#include "strplace.h"
#include <stdio.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

#define DOUBLE_PRECISION 10

/* Error defs. */
typedef enum {
	NONE = 255,

	/* Those are system spesific events.
	 * Usually lexer uses them.
	 */
	_PDONE, /* Parse Done */
	_PAGAIN, /* Parse Again */

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

typedef enum {
	NO_RESULT = -128,
	FN_EVAL,
	INTEGER,
	FRACTION,
	BINARY,
	BOOL,
	MATRIX
} etype_t;

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
	char *ascii; /* The query */
	double (*func)(void);
	symbol_table_t symbol_table;

} __attribute__((packed)) ersl_t;

void parse_query(ersl_t *ersl);
void euler_init(void);
