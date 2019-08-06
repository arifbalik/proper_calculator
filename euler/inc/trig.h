#ifndef __TRIG__
#define __TRIG__

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

#endif /* __TRIG__ */
