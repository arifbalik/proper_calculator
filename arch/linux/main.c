#include "../../euler/inc/euler.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const **argv)
{
	char query[80], print[180];
	ersl_t result;
	while (1) {
		printf(">>>");
		scanf("%s", query);

		if (parse_query(query, &result) < 0) {
			printf("%s\n", serr(result.error));
		} else {
			switch (result.type) {
			case FRACTION:
				gcvt(result.result.fraction, 10, print);
				printf("%s\n", print);
				break;
			default:
				printf("unsupported result type!\n");
				break;
			}
		}
	}

	return 0;
}
