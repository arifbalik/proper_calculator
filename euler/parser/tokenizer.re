#include "grammar.h"
#include "tokenizer.h"
#include <stdlib.h> /* only for parser */
#include <stdio.h> /* temporary */

/* Just change of notation */
#define parse Parse
#define parse_free ParseFree
#define parse_alloc ParseAlloc

void *ParseAlloc(void *(*allocProc)(size_t));
void Parse(void *, int, double, ersl_t *);
void ParseFree(void *, void (*freeProc)(void *));

#define FILL_LEX(q) YYCURSOR = q

char *YYCURSOR;

static int lex(void)
{
	char *YYMARKER;

	// Settings
	/*!stags:re2c format = 'char *@@;'; */
	/*!re2c
		re2c:define:YYCTYPE = char;
		re2c:yyfill:enable = 0;


	// Expression Defs.

		int = [0-9]+;
		float = [0-9]+('.'[0-9]+)('e'[+-]?[0-9]+)? ;
		letter = [a-z];


	// Number Defs.

		int  { return symbol_table_append(INT, YYCURSOR); }
		float  { return symbol_table_append(FLOAT, YYCURSOR); }
		letter  { return symbol_table_append(LETTER, YYCURSOR); }

	// Arithmetic Operator and Misc. Defs.

		"+"  { return symbol_table_append(PLUS, YYCURSOR); }
		"-"  { return symbol_table_append(MINUS, YYCURSOR);  }
		"*"  { return symbol_table_append(MULT, YYCURSOR); }
		"/"  { return symbol_table_append(DIV, YYCURSOR); }
		"^"  { return symbol_table_append(EXP, YYCURSOR); }
		"!"  { return symbol_table_append(FACT, YYCURSOR); }
		"#"  { return symbol_table_append(MOD, YYCURSOR); }
		"("  { return symbol_table_append(LPAREN, YYCURSOR); }
		")"  { return symbol_table_append(RPAREN, YYCURSOR); }

	// Boolean Operators

		"&"  { return symbol_table_append(AND, YYCURSOR); }
		"|"  { return symbol_table_append(OR, YYCURSOR); }
		"~"  { return symbol_table_append(NOT, YYCURSOR); }

	// System Defs.

		"\x00"  { return symbol_table_append(EOQ, YYCURSOR); }
		* { return symbol_table_append(UNKNOWN, YYCURSOR); }

	*/
}

static void clear_ersl(ersl_t *ersl)
{
	/* reset euler result */
	ersl->status = 0;
	ersl->type = 0;
	ersl->resultn.fraction = 0;
}

void parser_restart(void *parser)
{
	parse_free(parser, free);
	parser = parse_alloc(malloc);
}

void parse_query(char *query, ersl_t *ersl)
{
	void *parser = parse_alloc(malloc);
	char tmp[MAX_QUERY_LENGTH];

	clear_ersl(ersl);
	symbol_table_clear();

	FILL_LEX(query);

	symbol_table_init(query);

	printf("token \t| string \t| float\n");
	printf("rsv \t| NULL \t\t|NaN\n");

	while (lex() != EOQ) {
		parse(parser, get_last_matched_token(), get_if_double(), ersl);
		get_last_token_string(tmp);
		printf("%d \t| %s \t\t|%f\n", get_last_matched_token(), tmp,
		       get_if_double());
	}
	parse(parser, EOQ, 0, ersl);
	parse(parser, 0, 0, ersl);
	parse_free(parser, free);
}
