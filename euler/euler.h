#include <math.h>

/* Trig Functions. */
#define E_COS(num) cos(num)
#define E_ACOS(num) acos(num)
#define E_COSH(num) cosh(num)

#define E_SIN(num) sin(num)
#define E_ASIN(num) asin(num)
#define E_SINH(num) sinh(num)

#define E_TAN(num) tan(num)
#define E_ATAN(num) atan(num)
#define E_TANH(num) tanh(num)

#define E_COT(num) E_COS(num) / E_SIN(num)

/* Exponential and Logarithmic Functions. */
#define E_EXP(num) exp(num)
#define E_LOG(num) log(num)
#define E_LN(num) log10(num)

/* Power functions */
#define E_POW(x, y) pow(x, y)
#define E_SQRT(num) sqrt(num)
#define E_CBRT(num) cbrt(num)

/* Rounding and remainder functions */
#define E_CEIL(num) ceil(num)
#define E_FLOOR(num) floor(num)
#define E_MOD(x, y) fmod(x, y)

#define E_ABS(num) fabs(num)
