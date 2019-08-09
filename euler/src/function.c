#include "../inc/euler.h"

typedef struct {
	char *query;
	char dvar;
} function;

/* a function only has one letter name so naturally max number of functions is 26 */
static function fn[26];

static char *fn_place(char *query, const char *rep_o, const char *rep_n,
		      int8_t err)
{
	char *result;
	int i, cnt = 0;
	int rep_nlen;
	int rep_olen;

	if (rep_n != NULL && rep_o != NULL) {
		rep_nlen = strlen(rep_n);
		rep_olen = strlen(rep_o);
	} else {
		err = EMPTYQ;
		return query;
	}

	// Counting the number of times old word
	// occur in the string
	for (i = 0; query[i] != '\0'; i++) {
		if (strstr(&query[i], rep_o) == &query[i]) {
			cnt++;

			// Jumping to index after the old word.
			i += rep_olen - 1;
		}
	}

	// Making new string of enough length
	result = (char *)malloc(i + cnt * (rep_nlen - rep_olen) + 1);

	i = 0;
	while (*query) {
		// compare the substring with the result
		if (strstr(query, rep_o) == query) {
			strcpy(&result[i], rep_n);
			i += rep_nlen;
			query += rep_olen;
		} else
			result[i++] = *query++;
	}
	result[i] = '\0';
	return result;
}

int8_t fnexp(char *o1, char *o2, char *o3, char *query)
{
	char fname = o1[0];
	uint8_t idx = (uint8_t)(fname - 97);
	int8_t err = 0;
	if (fn[idx].dvar == '\0')
		return FNF;
	char *dvar = (char *)malloc(sizeof(char) + 1);
	char *fncall = (char *)malloc(sizeof(char) * (int8_t)(o3 - o1) + 1);
	char *fnparam = (char *)malloc(sizeof(char) * (int8_t)(o3 - o2) + 1);

	strncpy(dvar, &fn[idx].dvar, 1);
	*(dvar + 1) = '\0';

	/* First we have to expand the function from the form of f([number])
	 * to the function content such as : x*2+1. Then we'll subsitute x with fnparam.
	 * o1, o2 and o3 shows the beggining of certaion parts offunction calls
	 * [function name] -> o1
	 * [value to evaluate] -> o2
	 * end -> o3
	 */

	strncpy(fncall, o1, (o3 - o1));
	strncpy(fnparam, o2, (o3 - o2));

	fncall[(int8_t)(o3 - o1)] = '\0';
	fnparam[(int8_t)(o3 - o2)] = '\0';

	strcpy(query, fn_place(query, fncall, fn[idx].query, err));

	strcpy(query, fn_place(query, dvar, fnparam, err));

	free(fnparam);
	free(fncall);
	free(dvar);

	return err;
}

int8_t add_func(char fname, char dvar, char *query)
{
	uint8_t idx = (uint8_t)(fname - 97);
	uint8_t len;

	if (query == NULL)
		return EMPTYQ;

	len = strlen(query);

	fn[idx].query = (char *)malloc((sizeof(char) * len) + 3);
	fn[idx].query[len + 2] = '\0';

	fn[idx].query[0] = '(';
	strncpy(&fn[idx].query[1], query, len);
	fn[idx].query[len + 1] = ')';

	fn[idx].dvar = dvar;

	return 0;
}
