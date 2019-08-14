#include "../../euler/inc/euler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE 80

int main(int argc, char const **argv)
{
	char query[LINE + 1] = { '\0' };
	ersl_t euler;
	int c, k;

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
		if (euler.type == FRACTION)
			printf("result : %f\n", euler.resultn.fraction);
		else if (euler.type == BINARY) {
			printf("result : ");
			for (c = 31; c >= 0; c--) {
				k = (int)euler.resultn.fraction >> c;

				if (k & 1)
					printf("1");
				else
					printf("0");
			}

			printf("\n");
		}
	}

	return 0;
}
