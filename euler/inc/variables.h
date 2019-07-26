/*
        File info
 */
#ifndef __VARIABLES__
#define __VARIABLES__

void list_vars(void);
int lookup_var(char *vname);
double add_var(char *vname, double vval);
double get_var_val(char *vname, int idx);
void get_var_name(int idx);

/* TO FIX.
           this array holds the first variable name in
           the query line. Somehow parser recognize the last variable name in a line.
           For example for the following rule and input;
           rule: expr(A) ::= VAR(B) EQUAL expr(C)
           input: A=2*B+C
           when we call A.name (which is a struct) we get the name of C.
           It is maybe because the structre.
           */
static char fvar[MAX_VARNAME_LEN];

#endif /* __VARIABLES__ */
