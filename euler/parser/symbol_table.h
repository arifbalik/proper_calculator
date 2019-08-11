#ifndef __SYMBOL_TABLE__
#define __SYMBOL_TABLE__

typedef struct {
	uint8_t token;

	/* This holds the beggininng of matched token.
	 * Look for next index to get the end
	 */
	char *p;
} symbol_table_t;

void symbol_table_clear(void);
int8_t symbol_table_get(uint8_t idx, char *addr);
int8_t symbol_table_append(uint8_t token, char *addr);
int8_t get_last_matched_token(void);
double get_double_value(void);
void symbol_table_init(char *addr);
void get_string_value(char *target, char *p, uint8_t n);

#endif /* __SYMBOL_TABLE__ */
