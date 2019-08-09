#include "../../euler/inc/euler.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const **argv)
{
	char *query = (char *)malloc(sizeof(char) * 80);
	ersl_t euler;
	uint8_t i = 0;
	while (1) {
		i = 0;
		memset(query, '\0', 80);
		printf(">>>");
		while ((query[i] = getchar()) != EOF && query[i] != '\n') {
			if (query[i] != ' ') {
				i++;
			}
		}
		query[i] = '\0';
		printf("query %s, i: %d\n", query, i);
		parse_query(query, &euler);
		printf("result : %f\n", euler.resultn.fraction);
	}

	return 0;
}
