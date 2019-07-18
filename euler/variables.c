/*
        

 */
#include "euler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct symbol_table stbl[50];
static int vrc = 0;

void list(void)
{
	int i = vrc;
	if (i < 0)
		return;
	while (i--) {
		printf("Name: %s, vvalue: %f\n", stbl[i].name, stbl[i].val);
	}
}

int lookup(char *vname)
{
	int i = vrc - 1;
	if (i < 0)
		return 0;
	while (strcmp(vname, (stbl + i)->name) != 0) {
		i--;
		if (i < 0) {
			return -1;
		}
	}
	return i;
}

double get_var_val(char *vname)
{
	return stbl[lookup(vname)].val;
}

double add(char *vname, double vval)
{
	if (lookup(vname) != -1 && vrc > 0) {
		printf("Given Name: %s, index: %d, Found name:%s vval:%f\n",
		       vname, lookup(vname), (stbl + lookup(vname))->name,
		       (stbl + lookup(vname))->val);
		(stbl + lookup(vname))->val = vval;
		printf("Updated Value: %f\n", (stbl + lookup(vname))->val);
		return vval;
	}

	(stbl + vrc)->name = (char *)malloc(sizeof(char) * MAX_VARNAME_LEN);
	strcpy((stbl + vrc)->name, vname);
	(stbl + vrc)->val = vval;
	vrc++;
}
