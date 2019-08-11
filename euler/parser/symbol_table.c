#include "../inc/euler.h"

static symbol_table_t symbol_table[MAX_QUERY_LENGTH];
static uint8_t table_index = 0;

void symbol_table_clear(void)
{
	uint8_t i = MAX_QUERY_LENGTH - 1;
	while (i >= 0 && i < MAX_QUERY_LENGTH) {
		symbol_table[i].token = 0;
		symbol_table[i].p = NULL;
		i--;
	}
}

uint8_t symbol_table_get(uint8_t idx, char *addr)
{
	if (symbol_table[idx].p == NULL) {
		printf("symnf\n");
		return SYMNF;
	}

	addr = symbol_table[idx].p;

	return symbol_table[idx].token;
}
uint8_t symbol_table_append(uint8_t token, char *addr)
{
	if (table_index >= MAX_QUERY_LENGTH) {
		return ERROR;
	}

	symbol_table[table_index].token = token;
	symbol_table[table_index].p = addr;

	table_index++;

	if (token == EOL)
		table_index = 0;

	return token;
}
