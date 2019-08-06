#ifndef __FUNCTION__
#define __FUNCTION__

#define MAX_FUNCTION_QTY 10

typedef struct {
	double var;
	double result;
} fn_eval_table;

int8_t add_func(char fname, char dvar, char *query);
int8_t fnexp(char *o1, char *o2, char *o3, char *query);

#endif /* __FUNCTION__ */
