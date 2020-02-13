#include "grammar.h"
#include "tokenizer.h"
#include "symbol_table.h"
#include "ast.h"
#include "../inc/_atof.h"
#include "../inc/_itoa.h"
#include "../inc/_strcpy.h"
#include "../inc/strplace.h"

#define PARSE_TRACE

char *YYCURSOR;

#define TOKENIZE_AND_FILL_ST(query, e)                                         \
	init_parser(e, query);                                                 \
	while (lex(&(e->symbol_table)) != EOQ)

#define PUSH_TOKEN_STREAM_TO_PARSER(st, e, p)                                  \
	token = 0;                                                             \
	while (token != EOQ) {                                                 \
		token = st_get_next_token(st, p);                              \
		parse(parser, token, NULL, e);                                 \
	}                                                                      \
	FINISH_PARSING(e)

#define FINISH_PARSING(e) parse(parser, 0, NULL, e)

void fill_lex(char *q)
{
	YYCURSOR = q;
}

int lex(symbol_table_t *symbol_table)
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


	// Verbs.
		"to" { return st_append(symbol_table, TO, YYCURSOR, __no_priority); }

	// Iterated Functions.

		"sum" { return st_append(symbol_table, SIGMA, YYCURSOR, __func_begin); }
		"prod" { return st_append(symbol_table, PROD, YYCURSOR, __func_begin); }

	// Number Defs. and Constants

		"pi" { return st_append(symbol_table, PI, YYCURSOR, __no_priority); }
		("e")  { return st_append(symbol_table, E, YYCURSOR, __no_priority); }

		int  { return st_append(symbol_table, INT, YYCURSOR, __no_priority); }
		float  { return st_append(symbol_table, FLOAT, YYCURSOR, __no_priority); }
		letter  { return st_append(symbol_table, LETTER, YYCURSOR, __no_priority); }

	// Arithmetic Operator and Misc. Defs.

		"+"  { return st_append(symbol_table, PLUS, YYCURSOR, __no_priority); }
		"-"  { return st_append(symbol_table, MINUS, YYCURSOR, __no_priority);  }
		"*"  { return st_append(symbol_table, MULT, YYCURSOR, __no_priority); }
		"/"  { return st_append(symbol_table, DIV, YYCURSOR, __no_priority); }
		"^"  { return st_append(symbol_table, EXP, YYCURSOR, __no_priority); }
		"!"  { return st_append(symbol_table, FACT, YYCURSOR, __no_priority); }
		"#"  { return st_append(symbol_table, MOD, YYCURSOR, __no_priority); }
		"("  { return st_append(symbol_table, LPAREN, YYCURSOR, __no_priority); }
		")"  { return st_append(symbol_table, RPAREN, YYCURSOR, __no_priority); }
		","  { return st_append(symbol_table, COMMA, YYCURSOR, __no_priority); }
		"="  { return st_append(symbol_table, EQ, YYCURSOR, __no_priority); }

	// Boolean Operators

		"&"  { return st_append(symbol_table, AND, YYCURSOR, __no_priority); }
		"|"  { return st_append(symbol_table, OR, YYCURSOR, __no_priority); }
		"~"  { return st_append(symbol_table, NOT, YYCURSOR, __no_priority); }
		"(+)" { return st_append(symbol_table, XOR, YYCURSOR, __no_priority); }

	// Binary Relations

		"<" { return st_append(symbol_table, SMALLER, YYCURSOR, __no_priority); }
		">" { return st_append(symbol_table, GREATER, YYCURSOR, __no_priority); }
		"==" { return st_append(symbol_table, ISEQ, YYCURSOR, __no_priority); }
		"&&" { return st_append(symbol_table, BAND, YYCURSOR, __no_priority); }
		"||" { return st_append(symbol_table, BOR, YYCURSOR, __no_priority); }

	// System Defs.

		"\x00"  { return st_append(symbol_table, EOQ, YYCURSOR, __no_priority); }
		* { return st_append(symbol_table, UNKNOWN, YYCURSOR, __no_priority); }

	*/
}

void init_parser(ersl_t *euler, char *q)
{
	euler->status = NONE;

	st_init(&(euler->symbol_table), q);
	ast_init(euler);
	fill_lex(q);
}

void parse_query(ersl_t *euler)
{
	uint8_t token = 0;
	void *parser = parse_alloc();

	init_parser(euler, euler->ascii);

	/* Create symbol table */
	TOKENIZE_AND_FILL_ST(euler->ascii, euler);
#ifdef PARSE_TRACE
	ParseTrace(stdout, "parser >>");
	st_print(&(euler->symbol_table));
#endif

	PUSH_TOKEN_STREAM_TO_PARSER(&(euler->symbol_table), euler, 0);
}
