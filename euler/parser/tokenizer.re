#include "grammar.h"
#include "tokenizer.h"
#include <stdio.h>

/* Just change of notation */
#define parse Parse
#define parse_free ParseFree
#define parse_alloc ParseAlloc

void *ParseAlloc(void);
void Parse(void *, int, double, ersl_t *);
void ParseFree(void *);

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


	// Verbs.
		"to" { return symbol_table_append(TO, YYCURSOR); }

	// Iterated Functions.

		"sum" { return symbol_table_append(SIGMA, YYCURSOR); }

	// Number Defs. and Constants

		"pi" { return symbol_table_append(PI, YYCURSOR); }
		("e")  { return symbol_table_append(E, YYCURSOR); }

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
		","  { return symbol_table_append(COMMA, YYCURSOR); }
		"="  { return symbol_table_append(EQ, YYCURSOR); }

	// Boolean Operators

		"&"  { return symbol_table_append(AND, YYCURSOR); }
		"|"  { return symbol_table_append(OR, YYCURSOR); }
		"~"  { return symbol_table_append(NOT, YYCURSOR); }
		"(+)" { return symbol_table_append(XOR, YYCURSOR); }

	// Binary Relations

		"<" { return symbol_table_append(SMALLER, YYCURSOR); }
		">" { return symbol_table_append(GREATER, YYCURSOR); }
		"==" { return symbol_table_append(ISEQ, YYCURSOR); }
		"&&" { return symbol_table_append(BAND, YYCURSOR); }
		"||" { return symbol_table_append(BOR, YYCURSOR); }

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
	//ersl->resultn.fraction = 0;
}

void parse_query(ersl_t *ersl)
{
	void *parser;
	char tmp[MAX_QUERY_LENGTH];

	parser = parse_alloc();

	clear_ersl(ersl);
	symbol_table_clear();

	FILL_LEX(ersl->ascii);

	symbol_table_init(ersl->ascii);

	//printf("token \t| string \t| float\nrsv \t| NULL \t\t|NaN\n");
	while (lex() != EOQ) {
		parse(parser, get_last_matched_token(), get_if_number(), ersl);
		get_last_token_string(tmp);
		printf("%d \t| %s \t\t|%f\n", get_last_matched_token(), tmp,
		       get_if_number());
	}

	parse(parser, EOQ, 0, ersl);
	parse(parser, 0, 0, ersl);
	parse_free(parser);
}
