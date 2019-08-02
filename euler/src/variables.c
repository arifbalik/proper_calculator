#include "../inc/euler.h"
#include "../inc/variables.h"

typedef struct {
	char name[MAX_VARNAME_LEN];
	double val;
} variable_table;

static variable_table vrbv[MAX_VAR_QTY];
static uint8_t vrbc = 0;

static int lookup_vars(char *vname);

char *serr(int8_t err)
{
	switch (err) {
	case MXVQ:
		return strdup("Max variable length has been reached!");
	case NVF:
		return strdup("Not found!");
	default:
		return strdup("Unkown error!");
	}
}

void list_vars(void)
{
	register int8_t idx = vrbc;
	if (idx < 0)
		return;
	while (idx--) {
		/* TODO: implement an algorithm that does not involve printing in here */
	}
}
static int lookup_vars(char *vname)
{
	register int8_t idx = vrbc - 1;
	while (strcmp(vname, vrbv[idx].name) != 0) {
		idx--;
		if (idx < 0)
			return idx;
	}
	return idx; /* it returns -1 if no variable found */
}

int8_t get_var_val(char *vname, int8_t idx, double *val)
{
	if (idx >= 0) { /* if idx > 0 function returns directly with id*/
		*val = vrbv[idx].val;
	}
	idx = lookup_vars(vname);

	/* No variable found */
	if (idx < 0) {
		return NVF;
	}

	*val = vrbv[idx].val;
	return 0;
}

int8_t add_var(char *vname, double vval)
{
	register int8_t idx = -1; /* arbitrary negative value */

	/* Check for availability */
	if (vrbc >= MAX_VAR_QTY) {
		return MXVQ;
	}

	/* Update if variable already exists. */
	idx = lookup_vars(vname);
	if (idx >= 0 && vrbc > 0) {
		vrbv[idx].val = vval;
		return 0;
	}

	strcpy(vrbv[vrbc].name, vname);
	vrbv[vrbc].val = vval;
	vrbc++;

	return 0;
}
