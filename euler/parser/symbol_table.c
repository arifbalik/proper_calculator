#include "../inc/euler.h"
#include "grammar.h"

static symbol_table_t symbol_table[MAX_QUERY_LENGTH];
static uint8_t table_index = 0;

void symbol_table_init(char *addr)
{
	symbol_table[0].p = addr;
	symbol_table[0].token = 0;
}

void symbol_table_clear(void)
{
	uint8_t i = MAX_QUERY_LENGTH - 1;
	while (i >= 0 && i < MAX_QUERY_LENGTH) {
		symbol_table[i].token = 0;
		symbol_table[i].p = NULL;
		i--;
	}
}

int8_t symbol_table_get(uint8_t idx, char *addr)
{
	if (symbol_table[idx].p == NULL) {
		return -1;
	}

	addr = symbol_table[idx].p;

	return symbol_table[idx].token;
}
int8_t symbol_table_append(uint8_t token, char *addr)
{
	table_index++;

	if (table_index >= MAX_QUERY_LENGTH) {
		return -1;
	}

	symbol_table[table_index].token = token;
	symbol_table[table_index].p = addr;

	/* EOQ check */
	if (token == EOQ) {
		table_index = 0;
		return EOQ;
	}

	return token;
}

int8_t get_last_matched_token(void)
{
	return symbol_table[table_index].token;
}

int8_t get_token(uint8_t idx)
{
	return symbol_table[idx].token;
}

double get_if_number(void)
{
	char sval[MAX_QUERY_LENGTH];

	if (symbol_table[table_index].token != INT &&
	    symbol_table[table_index].token != FLOAT) {
		return 0;
	}

	if (table_index >= 1) {
		get_last_token_string(sval);
		return _atof(sval);
	}

	else
		return 0;
}

void get_last_token_string(char *target)
{
	get_string(target, symbol_table[table_index - 1].p,
		   (uint8_t)(symbol_table[table_index].p -
			     symbol_table[table_index - 1].p));
}
void get_string(char *target, char *p, uint8_t n)
{
	uint8_t idx = 0;

	while (n) {
		*(target + idx) = *(p + idx);
		idx++;
		n--;
	}
	*(target + idx) = '\0';
}
