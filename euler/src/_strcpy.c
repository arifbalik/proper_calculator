#include <stdint.h>

void _strcpy(char *tar, char *dest, uint8_t max)
{
	uint8_t idx = 0;
	while ((dest + idx) && idx < max) {
		*(tar + idx) = *(dest + idx);
		idx++;
	}
	*(tar + idx) = '\0';
}

void _strnncpy(char *dest, char *src, uint8_t n)
{
	uint8_t idx = 0;
	while (idx < n && (src + idx)) {
		*(dest + idx) = *(src + idx);
		idx++;
	}
}
