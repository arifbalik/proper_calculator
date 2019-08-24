#include "../inc/euler.h"

char *_strcpy(char *tar, char *dest, uint8_t max)
{
	uint8_t idx = 0;

	while (idx < max && *(dest + idx) != '\0') {
		*(tar + idx) = *(dest + idx);
		idx++;
	}
	*(tar + idx) = '\0';

	return tar;
}

char *_strnncpy(char *dest, char *src, uint8_t n)
{
	uint8_t idx = 0;
	while (idx < n && (src + idx)) {
		*(dest + idx) = *(src + idx);
		idx++;
	}
	return dest;
}
