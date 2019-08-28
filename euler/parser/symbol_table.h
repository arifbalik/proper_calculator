#ifndef __SYMBOL_TABLE__
#define __SYMBOL_TABLE__

void st_init(symbol_table_t *symbol_table, char *addr);
uint8_t st_append(symbol_table_t *symbol_table, uint8_t token, char *addr,
		  int8_t priority);

uint8_t st_get_next_token(symbol_table_t *symbol_table, uint8_t ispartial);
double st_get_number(symbol_table_t *symbol_table);
char st_get_letter(symbol_table_t *symbol_table);
void st_get_token_string(symbol_table_t *symbol_table, char *target,
			 uint8_t idx);

uint8_t st_count_token(symbol_table_t *symbol_table, uint8_t token);
void st_get_string_between_tokens(symbol_table_t *symbol_table, uint8_t ftoken,
				  uint8_t ltoken, char *s, uint8_t ispartial);

void st_set_cur(symbol_table_t *symbol_table, uint8_t idx);
void st_reset_cur(symbol_table_t *symbol_table);

uint8_t st_markdown_func(symbol_table_t *symbol_table, char *query);
void st_print(symbol_table_t *symbol_table);

#endif /* __SYMBOL_TABLE__ */
