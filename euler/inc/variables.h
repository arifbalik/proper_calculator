#ifndef __VARIABLES__
#define __VARIABLES__

/* Symbol table defines */
#define MAX_VAR_QTY 10
#define MAX_VARNAME_LEN 10

void list_vars(void);
int lookup_var(char *vname);
int8_t add_var(char *vname, double vval);
int8_t get_var_val(char *vname, int8_t idx, double *val);
char *serr(int8_t err);

#endif /* __VARIABLES__ */
