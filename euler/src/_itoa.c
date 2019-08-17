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
