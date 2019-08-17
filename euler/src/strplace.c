#include "../inc/euler.h"

void strplace(char *s, char *dest, char old, int number)
{
	char temp[MAX_QUERY_LENGTH], new[MAX_QUERY_LENGTH];
	uint8_t lenq = 0, lenn = 0, i = 0, k = 0;

	_strcpy(dest, s, MAX_QUERY_LENGTH);
	lenq = _strlen(dest, MAX_QUERY_LENGTH);

	_itoa(number, new);
	lenn = _strlen(new, MAX_QUERY_LENGTH);

	while (k < lenq) {
		if (*(dest + k) == old) {
			_strnncpy((temp + i), new, lenn);
			i += lenn - 1;
		} else {
			*(temp + i) = *(dest + k);
		}
		i++;
		k++;
	}
	temp[i] = '\0';

	_strcpy(dest, temp, MAX_QUERY_LENGTH);
}
