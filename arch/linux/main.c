#include "../../euler/inc/euler.h"
#include <stdio.h>
#include <stdlib.h>

#define LINE 80

int main(int argc, char const **argv)
{
	char query[LINE + 1] = { '\0' };
	ersl_t euler;

	while (1) {
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

		parse_query(query, &euler);
		printf("result : %f\n", euler.resultn.fraction);
	}

	return 0;
}
