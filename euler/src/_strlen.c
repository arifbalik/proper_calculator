#include "../inc/euler.h"

uint8_t _strlen(char *s, uint8_t max)
{
	uint8_t idx = 0;
	while (*(s + idx) != '\0' && idx < max)
		idx++;

	return idx;
}
