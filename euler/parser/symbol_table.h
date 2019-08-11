#ifndef __SYMBOL_TABLE__
#define __SYMBOL_TABLE__

typedef struct {
	uint8_t token;

	/* This holds the beggininng of matched token.
	 * Look for next index to get the end
	 */
	char *p;
} symbol_table_t;

#endif /* __SYMBOL_TABLE__ */
