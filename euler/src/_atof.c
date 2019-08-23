#include "../inc/euler.h"

double _atof(const char *s)
{
	double rez = 0, fact = 1;
	if (*s == '-') {
		s++;
		fact = -1;
	}
	for (int8_t point_seen = 0; *s; s++) {
		if (*s == '.') {
			point_seen = 1;
			continue;
		}
		int8_t d = *s - '0';
		if (d >= 0 && d <= 9) {
			if (point_seen)
				fact /= 10.0f;
			rez = rez * 10.0f + (double)d;
		}
	}
	return rez * fact;
}
