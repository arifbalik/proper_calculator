#include "euler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"

struct symbol_table stbl[MAX_VAR_QTY];
static int vrc = 0;
char *errmsg;

int terr(int code, char *msg)
{
	printf("euler(error): %s\n", msg);
	__err = MXVQ;
	return __err;
}
void list(void)
{
	int idx = vrc;
	if (idx < 0)
		return;
	while (idx--) {
		printf("Name: %s, vvalue: %f\n", stbl[idx].name, stbl[idx].val);
	}
}
int lookup(char *vname)
{
	int idx = vrc - 1;
	if (idx < 0)
		return NVF;
	while (strcmp(vname, (stbl + idx)->name) != 0) {
		idx--;
		if (idx < 0)
			return NVF;
	}
	return idx;
}

double get_var_val(char *vname)
{
	int idx = lookup(vname);
	if (idx < 0) {
		errmsg = (char *)malloc(sizeof(char) * 30);
		sprintf(errmsg, "No variable found named: %s", vname);
		terr(NVF, errmsg);
		free(errmsg);
		return 0;
	} else {
		return stbl[idx].val;
	}
}

double add(char *vname, double vval)
{
	int idx = -1; /* arbitrary negative value */
	if (vrc >= MAX_VAR_QTY) {
		terr(MXVQ, "Max variable quantity has been reached!");
		return MXVQ;
	}
	idx = lookup(vname);
	if (idx >= 0 && vrc > 0) {
		(stbl + idx)->val = vval;
		return vval;
	}

	(stbl + vrc)->name = (char *)malloc(sizeof(char) * MAX_VARNAME_LEN);
	strcpy((stbl + vrc)->name, vname);
	(stbl + vrc)->val = vval;
	vrc++;
}
