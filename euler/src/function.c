#include "../inc/euler.h"

typedef struct {
	char *query;
	char dvar;
} function;

/* a function only has one letter name so naturally max number of functions is 26 */
static function fn[26];

static char *fn_place(char *s, const char *oldW, const char *newW)
{
	char *result;
	int i, cnt = 0;
	int newWlen = strlen(newW);
	int oldWlen = strlen(oldW);

	// Counting the number of times old word
	// occur in the string
	for (i = 0; s[i] != '\0'; i++) {
		if (strstr(&s[i], oldW) == &s[i]) {
			cnt++;

			// Jumping to index after the old word.
			i += oldWlen - 1;
		}
	}

	// Making new string of enough length
	result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1);

	i = 0;
	while (*s) {
		// compare the substring with the result
		if (strstr(s, oldW) == s) {
			strcpy(&result[i], newW);
			i += newWlen;
			s += oldWlen;
		} else
			result[i++] = *s++;
	}
	result[i] = '\0';
	return result;
}

void fnevn(char fname, double num, double *result)
{
	uint8_t idx = (uint8_t)(fname - 97);
}
int8_t fnexp(char *o1, char *o2, char *o3, char *query)
{
	char fname = o1[0];
	uint8_t idx = (uint8_t)(fname - 97);
	if (fn[idx].dvar == '\0')
		return FNF;

	char *dvar = (char *)malloc(sizeof(char));
	char *fncall = (char *)malloc(sizeof(char) * (int8_t)(o3 - o1));
	char *fnparam = (char *)malloc(sizeof(char) * (int8_t)(o3 - o1));

	strncpy(dvar, &fn[idx].dvar, 1);
	*(dvar + 1) = '\0';

	/* First we have to expand the function from the form of f([number])
	 * to the function content such as : x*2+1. Then we'll subsitute x with fnparam.
	 * o1, o2 and o3 shows the beggining of certaion parts offunction calls
	 * [function name] -> o1
	 * [value to evaluate] -> o2
	 * end -> o3
	 */

	memset(fncall, '\0', sizeof(fncall));
	memset(fnparam, '\0', sizeof(fnparam));

	strncpy(fncall, o1, (o3 - o1));
	strncpy(fnparam, o2, (o3 - o2));

	strcpy(query, fn_place(query, fncall, fn[idx].query));
	// printf("expand: %s, eval: %s, dvar: %s, o3-o1: %d\n", query, fncall,
	//        fn[idx].query, (int8_t)(o3 - o1));

	strcpy(query, fn_place(query, dvar, fnparam));
	//printf("subs.: %s, eval: %s, dvar: %s\n", query, fnparam, dvar);

	free(fnparam);
	free(fncall);
	free(dvar);

	return 0;
}

int8_t add_func(char fname, char dvar, char *query)
{
	uint8_t idx = (uint8_t)(fname - 97);

	fn[idx].query = (char *)malloc(sizeof(char) * strlen(query));

	fn[idx].query[0] = '(';
	strcpy(&fn[idx].query[1], query);
	fn[idx].query[strlen(fn[idx].query)] = ')';
	fn[idx].query[strlen(fn[idx].query) + 1] = '\0';

	fn[idx].dvar = dvar;

	return 0;
}
