#include "../inc/euler.h"

int strstr_compare(char *x, char *y)
{
	while (*x && *y) {
		if (*x != *y)
			return 0;

		x++;
		y++;
	}

	return (*y == '\0');
}

char *_strstr(char *x, char *y)
{
	while (*x != '\0') {
		if ((*x == *y) && strstr_compare(x, y))
			return x;
		x++;
	}

	return NULL;
}
