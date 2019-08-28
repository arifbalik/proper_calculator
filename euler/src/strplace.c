#include "../inc/euler.h"
#include "../inc/_strcpy.h"
#include "../inc/_strlen.h"
#include "../inc/_strstr.h"
#include "../inc/_itoa.h"

void strplace(char *s, char *dest, char old, int number)
{
	char temp[MAX_QUERY_LENGTH], new[MAX_QUERY_LENGTH];
	uint8_t lenq = 0, lenn = 0, i = 0, k = 0;

	_strcpy(dest, s, MAX_QUERY_LENGTH);
	lenq = _strlen(dest, MAX_QUERY_LENGTH);

	ftoa(number, new, DOUBLE_PRECISION);
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
char *str_replace(char *s, char *oldW, char *newW, char *result)
{
	int i;
	int newWlen = _strlen(newW, MAX_QUERY_LENGTH);
	int oldWlen = _strlen(oldW, MAX_QUERY_LENGTH);

	// Counting the number of times old word
	// occur in the string
	for (i = 0; s[i] != '\0'; i++) {
		if (_strstr(&s[i], oldW) == &s[i]) {
			// Jumping to index after the old word.
			i += oldWlen - 1;
		}
	}

	i = 0;
	while (*s) {
		// compare the substring with the result
		if (_strstr(s, oldW) == s) {
			_strcpy(&result[i], newW, MAX_QUERY_LENGTH);
			i += newWlen;
			s += oldWlen;
		} else
			result[i++] = *s++;
	}

	result[i] = '\0';
	return result;
}
