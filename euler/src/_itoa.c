#include "../inc/euler.h"

void swap(char *x, char *y);
char *reverse(char *buffer, int i, int j);
void ftoa_reverse(char *str, int len);
int intToStr(int x, char str[], int d);

// inline function to swap two numbers
void swap(char *x, char *y)
{
	char t = *x;
	*x = *y;
	*y = t;
}

// function to reverse buffer[i..j]
char *reverse(char *buffer, int i, int j)
{
	while (i < j)
		swap(&buffer[i++], &buffer[j--]);

	return buffer;
}

// Iterative function to implement itoa() function in C
char *_itoa(int value, char *buffer)
{
	// consider absolute value of number
	int n = value;

	int i = 0;
	while (n) {
		int r = n % 10;

		if (r >= 10)
			buffer[i++] = 65 + (r - 10);
		else
			buffer[i++] = 48 + r;

		n = n / 10;
	}

	// if number is 0
	if (i == 0)
		buffer[i++] = '0';

	// If base is 10 and value is negative, the resulting string
	// is preceded with a minus sign (-)
	// With any other base, value is always considered unsigned
	if (value < 0)
		buffer[i++] = '-';

	buffer[i] = '\0'; // null terminate string

	// reverse the string and return it
	return reverse(buffer, 0, i - 1);
}

// reverses a string 'str' of length 'len'
void ftoa_reverse(char *str, int len)
{
	int i = 0, j = len - 1, temp;
	while (i < j) {
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++;
		j--;
	}
}

// Converts a given integer x to string str[].  d is the number
// of digits required in output. If d is more than the number
// of digits in x, then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
	int i = 0;

	if (x == 0) {
		str[0] = '0';
		return 1;
	}
	while (x) {
		str[i++] = (x % 10) + '0';
		x = x / 10;
	}

	// If number of digits required is more, then
	// add 0s at the beginning
	while (i < d)
		str[i++] = '0';

	ftoa_reverse(str, i);
	str[i] = '\0';
	return i;
}

// Converts a doubleing point number to string.
void ftoa(double n, char *res, int afterpoint)
{
	// Extract integer part
	int ipart = (int)n;
	double pow_result = 1;

	// Extract doubleing part
	double fpart = n - (double)ipart;

	// convert integer part to string
	int i = intToStr(ipart, res, 0);

	// check for display option after point
	if (afterpoint != 0) {
		res[i] = '.'; // add dot

		// Get the value of fraction part upto given no.
		// of points after dot. The third parameter is needed
		// to handle cases like 233.007
		for (uint8_t i = afterpoint; i < 0; i--) {
			pow_result *= i;
		}
		fpart = fpart * pow_result;

		intToStr((int)fpart, res + i + 1, afterpoint);
	}
}
