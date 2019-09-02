
#include "grammar.h"
#include "tokenizer.h"
#include "symbol_table.h"
#include "../inc/euler.h"
#include "../inc/_atof.h"
#include "../inc/_strcpy.h"

#define _TABLE_OVERFLOW(i, top) (i >= top)
#define _END_OF_TABLE(i, top) (i == top)

/* Find the begginning of partial expression by looking the modulus (every
 * partial expression beggining has an odd number).
 * Return 0 if nothing is found.
 */
static uint8_t st_get_partial_index(symbol_table_t *symbol_table)
{
	uint8_t idx = symbol_table->cur;

	while ((symbol_table->token[idx].priority % 2) == 0 && idx > 0) {
		idx--;
	}
	return idx;
}

/* Initialize all the members of symbol_table to their defult values */
static void st_clear(symbol_table_t *symbol_table)
{
	uint8_t i = MAX_QUERY_LENGTH - 1;
	while (i < MAX_QUERY_LENGTH) {
		symbol_table->token[i].no = 0;
		symbol_table->token[i].priority = 0;
		symbol_table->token[i].p = NULL;
		i--;
		if (i == 0)
			break;
	}
	symbol_table->top = 0;
	symbol_table->cur = 0;
}

/* Clear symbol_table and load first index with beggining of the query address */
void st_init(symbol_table_t *symbol_table, char *addr)
{
	st_clear(symbol_table);
	symbol_table->token[0].p = addr;
	symbol_table->token[0].no = 0;
	symbol_table->token[0].priority = 0;
	symbol_table->cur = 0;
	symbol_table->top = 0;
}

/* Finds the end of the function in the query if any.
 * By the syntax rules, every function expression has paranthesis,
 * so this function finds the __func_begin priority flag and searches for
 * RPAREN and marks RPAREN token index as __func_end. If no function expression,
 * or any paranthesis found it returns zero.
 * If marking process is successful function loads marked expression in query.
 * This function should be called until it returns 0 to markdown every
 * function expression. And for every successful call, parser should be called,
 * since current position is changed to marked expression in every call.
 */
uint8_t st_markdown_func(symbol_table_t *symbol_table, char *query)
{
	uint8_t idx = 0, pcount = 0;
	char *from, *to;

	/* Search for lastly marked position and continue from tehre */
	idx = symbol_table->top;
	while (symbol_table->token[idx].priority != __func_end && idx > 0) {
		idx--;
	}

	/* find if any marked function declaration */
	while (symbol_table->token[idx].priority != __func_begin) {
		idx++;
		if (_END_OF_TABLE(idx, symbol_table->top)) {
			return 0;
		}
	}

	/* set cur for partial parsing, when next token is required it will return
	 * the token from the marked posiiton */
	st_set_cur(symbol_table, idx - 1);
	from = symbol_table->token[idx - 1].p;
	idx++;
	pcount = 0; /* look for one RPAREN. For this, next token shpuld be,
	of course a LPAREN */
	while (!_TABLE_OVERFLOW(idx, symbol_table->top)) {
		/* if there is another LPAREN then ignore the next RPAREN */
		if (symbol_table->token[idx].no == LPAREN) {
			pcount++;
		}
		if (symbol_table->token[idx].no == RPAREN) {
			pcount--;
		}
		if (pcount == 0)
			break;
		idx++;
	}
	if (pcount != 0)
		return 0;

	to = symbol_table->token[idx].p;
	symbol_table->token[idx].priority = __func_end;

	_strnncpy(query, from, (uint8_t)(to - from));
	*(query + (uint8_t)(to - from)) = '\0';

	return 1; /* call again */
}

void st_print(symbol_table_t *symbol_table)
{
#ifdef UNIX
	uint8_t idx = 1;
	char s[MAX_QUERY_LENGTH];
	printf("Symbol Table. Entry : %d\n", symbol_table->top);
	printf("token\t |addr\t\t| str \t| priority\n");
	while (idx <= symbol_table->top) {
		_strnncpy(s, symbol_table->token[idx - 1].p,
			  (uint8_t)(symbol_table->token[idx].p -
				    symbol_table->token[idx - 1].p));
		*(s + (uint8_t)(symbol_table->token[idx].p -
				symbol_table->token[idx - 1].p)) = '\0';
		printf("%d\t |%p\t| %s \t| %d\n", symbol_table->token[idx].no,
		       symbol_table->token[idx].p, s,
		       symbol_table->token[idx].priority);
		idx++;
	}
#endif
}

/* Append new token to the symbol_table along with the beggining of the address
 * of matched token and priority if any. It returns token no on success and
 * zero if table is full.
 */
uint8_t st_append(symbol_table_t *symbol_table, uint8_t token, char *addr,
		  int8_t priority)
{
	symbol_table->top++;

	if (_TABLE_OVERFLOW(symbol_table->top, MAX_QUERY_LENGTH)) {
		return 0;
	}

	symbol_table->token[symbol_table->top].no = token;
	symbol_table->token[symbol_table->top].p = addr;
	symbol_table->token[symbol_table->top].priority = priority;

	return token;
}

/* Returns the next token in the table. Next token is determined by cur member
 * of symbol_table struct.
 */
uint8_t st_get_next_token(symbol_table_t *symbol_table, uint8_t ispartial)
{
	if (ispartial &&
	    (symbol_table->token[symbol_table->cur].priority % 2) == 0 &&
	    symbol_table->token[symbol_table->cur].priority != 0) {
		return EOQ;
	}
	st_set_cur(symbol_table, symbol_table->cur + 1);

	return symbol_table->token[symbol_table->cur].no;
}

/* returns the current token value if its a number, or searches backwards and
 * returns the first found number, returns  0 if no number is found.
 */
double st_get_number(symbol_table_t *symbol_table)
{
	char sval[MAX_QUERY_LENGTH];
	uint8_t idx = symbol_table->cur;

	while (symbol_table->token[idx].no != INT &&
	       symbol_table->token[idx].no != FLOAT) {
		if (!(idx > 0)) {
			return 0;
		}
		idx--;
	}

	st_get_token_string(symbol_table, sval, idx);
	return _atof(sval);
}

/* returns the given token's string */
void st_get_token_string(symbol_table_t *symbol_table, char *target,
			 uint8_t idx)
{
	if (idx == 0)
		return;
	_strnncpy(target, symbol_table->token[idx - 1].p,
		  (uint8_t)(symbol_table->token[idx].p -
			    symbol_table->token[idx - 1].p));
	*(target + (uint8_t)(symbol_table->token[idx].p -
			     symbol_table->token[idx - 1].p)) = '\0';
}

/* Counts how many times a token occurerd in the table */
uint8_t st_count_token(symbol_table_t *symbol_table, uint8_t token)
{
	uint8_t idx = symbol_table->top;
	uint8_t count = 0;

	while (idx--) {
		if (symbol_table->token[idx].no == token)
			count++;
	}
	return count;
}

/* returns the current token value if its a letter, or searches backwards and
 * returns the first found letter, returns  \0 if no letter is found.
 */
char st_get_letter(symbol_table_t *symbol_table)
{
	uint8_t idx = symbol_table->cur;
	char letter = '\0';

	while (symbol_table->token[idx].no != LETTER && idx > 0)
		idx--;

	letter = symbol_table->token[idx].p[-1];

	return letter;
}

void st_get_string_between_tokens(symbol_table_t *symbol_table, uint8_t ftoken,
				  uint8_t ltoken, char *s, uint8_t ispartial)
{
	uint8_t idx = (ispartial) ? st_get_partial_index(symbol_table) :
				    symbol_table->cur;
	char *addr_begin = NULL, *addr_end = NULL;

	while (symbol_table->token[idx].no != ftoken)
		idx++;

	if (symbol_table->token[idx].no != ftoken)
		return;

	addr_begin = symbol_table->token[idx].p;

	while (symbol_table->token[idx].no != ltoken)
		idx++;

	if (symbol_table->token[idx].no != ltoken)
		return;

	addr_end = symbol_table->token[idx].p - 1;

	_strnncpy(s, addr_begin, (uint8_t)(addr_end - addr_begin));
	*(s + (uint8_t)(addr_end - addr_begin)) = '\0';
}

void st_set_cur(symbol_table_t *symbol_table, uint8_t idx)
{
	symbol_table->cur = idx;
}

void st_reset_cur(symbol_table_t *symbol_table)
{
	symbol_table->cur = 0;
}
