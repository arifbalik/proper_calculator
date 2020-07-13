#include "../../euler/inc/euler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE 80

void test_euler_out(char *cmd, char *out)
{
	ersl_t *euler = _euler();

	euler->ascii = cmd;
	printf("parsing %s... ", cmd);
	parse_query();
	if (strcmp(out, euler->out) == 0) {
		printf("= %s [PASS]\n", euler->out);
	} else {
		printf("[FAIL] excpected %s but got %s\n", out, euler->out);
	}

	euler->out_pos = 0;

	printf("\n");
}

int main(int argc, char const **argv)
{
	ersl_t *euler = _euler();
	printf("size of euler %ld, size of symbol_table %ld, sizeof ast %ld\n",
	       (long)sizeof(*euler), (long)sizeof(euler->symbol_table),
	       (long)sizeof(euler->ast));
#ifdef _TEST_
	printf("\n\nEULER IS IN TEST MODE! \n\nTest start...\n\n");
	test_euler_out("2+2", "4");
	test_euler_out("2*2", "4");
	test_euler_out("(1+2+3+4)/7", "11/7");
	test_euler_out("(1+2+3+3)/11", "9/11");
	test_euler_out("(3*2*3)/7", "12/7");
	test_euler_out("2^2/4", "1");
#else
	while (1) {
		char query[LINE + 1] = { '\0' };
		uint8_t i = 0;
		char ch = 0;
		memset(query, '\0', LINE);
		printf(">>>");

		for (i = 0; (i < LINE) && ((ch = getchar()) != EOF) &&
			    (ch != '\n') && ch != '\0';
		     ++i) {
			if (ch != ' ')
				query[i] = ch;
			else
				i--;
		}

		query[i] = '\0'; /* a string should always end with '\0' ! */
		euler->ascii = query;
		parse_query();
	}
#endif
	while (1)
		;
	return 0;
}
