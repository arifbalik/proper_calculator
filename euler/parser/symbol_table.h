#ifndef __SYMBOL_TABLE__
#define __SYMBOL_TABLE__

typedef struct {
	/* This holds the beggininng of matched token.
	 * Look for next index to get the end
	 */
	char *p;

	uint8_t no; /* Hold token number, defined in grammar.h */
	uint8_t priority; /* For partial parsing */

} __attribute__((packed)) token_t;

typedef struct {
	token_t token[MAX_QUERY_LENGTH];
	uint8_t top; /* index of last element in the stack */
	uint8_t cur; /* currently handled token index */

} __attribute__((packed)) symbol_table_t;

void st_init(symbol_table_t *symbol_table, char *addr);
uint8_t st_append(symbol_table_t *symbol_table, uint8_t token, char *addr,
		  int8_t priority);

uint8_t st_get_next_token(symbol_table_t *symbol_table, uint8_t ispartial);
double st_get_number(symbol_table_t *symbol_table);
char st_get_letter(symbol_table_t *symbol_table);
void st_get_token_string(symbol_table_t *symbol_table, char *target);

uint8_t st_count_token(symbol_table_t *symbol_table, uint8_t token);
void st_get_string_between_tokens(symbol_table_t *symbol_table, uint8_t ftoken,
				  uint8_t ltoken, char *s, uint8_t ispartial);

void st_set_cur(symbol_table_t *symbol_table, uint8_t idx);
void st_reset_cur(symbol_table_t *symbol_table);

uint8_t st_markdown_func(symbol_table_t *symbol_table, char *query);
void st_print(symbol_table_t *symbol_table);

#endif /* __SYMBOL_TABLE__ */
