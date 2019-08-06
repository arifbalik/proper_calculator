#include "../../euler/inc/euler.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const **argv)
{
	char query[80], print[180], tmp;
	ersl_t euler;
	int i = 0;
	euler.type = 0;
	while (1) {
		i = 0;
		for (size_t i = 0; i < 80; i++) {
			query[i] = '\x00';
		}
		printf(">>>");
		while ((tmp = getc(stdin)) != '\n') {
			if (tmp == ' ') {
				continue;
			} else
				query[i] = tmp;
			i++;
		}
		parse_query(query, &euler);

		switch (euler.type) {
		case FRACTION:
			gcvt(euler.resultn.fraction, 10, print);
			printf("%s\n", print);
			break;
		case FN_EVAL:
			printf("var\t\t|val\n");
			for (size_t i = 0; i < 20; i++) {
				printf("%f\t|%f\n",
				       euler.resultn.fn_eval[i].var,
				       euler.resultn.fn_eval[i].result);
			}
			break;
		case NO_RESULT:
			printf("ok\n");
			break;
		default:
			printf("%s\n", serr(euler.status));
			break;
		}
	}

	return 0;
}
