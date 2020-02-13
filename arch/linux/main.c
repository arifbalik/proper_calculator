#include "../../euler/inc/euler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE 80

int main(int argc, char const **argv)
{
	char query[LINE + 1] = { '\0' };
	ersl_t euler;
	printf("size of euler %ld, size of symbol_table %ld, sizeof ast %ld\n",
	       (long)sizeof(euler), (long)sizeof(euler.symbol_table),
	       (long)sizeof(euler.ast));
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
		euler.ascii = query;
		parse_query(&euler);
	}

	return 0;
}
