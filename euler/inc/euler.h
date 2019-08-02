#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "variables.h"

/* Constants */
#ifndef M_PI
#define M_PI                                                                   \
	3.141592653589793238462643383279502884197169399375105820974944592307816406286208998
#endif

/* Trig Functions. */
#define E_COS(x) cos(x)
#define E_ACOS(x) acos(x)
#define E_COSH(x) cosh(x)

#define E_SIN(x) sin(x)
#define E_ASIN(x) asin(x)
#define E_SINH(x) sinh(x)

#define E_TAN(x) tan(x)
#define E_ATAN(x) atan(x)
#define E_TANH(x) tanh(x)

#define E_COT(x) E_COS(x) / E_SIN(x)

/* Exponential and Logarithmic Functions. */
#define E_EXP(x) exp(x)
#define E_LOG(x) log(x)
#define E_LN(x) log10(x)

/* Power functions */
#define E_POW(x, y) pow(x, y)
#define E_SQRT(x) sqrt(x)
#define E_CBRT(x) cbrt(x)

/* Rounding and remainder functions */
#define E_CEIL(x) ceil(x)
#define E_FLOOR(x) floor(x)
#define E_MOD(x, y) fmod(x, y)

#define E_ABS(x) fabs(x)

/* Error defs. */
typedef enum {
	NONE = 0,
	/* Euler std. errors */
	MTHE = CHAR_MIN, /* Math Error */

	/* Variable errors */
	MXVQ, /* Max Variable Quantity Has Been Reaced */
	NVF /* No Variable Found */
} elerr_t;

/* Results will be stored here by the parser */
typedef union {
	double fraction;
	double **matrix;
	char *var;
} eval_t;

/* Developer should check this type before any action */
typedef enum { INTEGER = CHAR_MIN, FRACTION, MATRIX } eobj_t;

typedef struct {
	eobj_t type;
	eval_t result;
	elerr_t error;
} ersl_t;

int parse_query(char *query, ersl_t *ersl);
