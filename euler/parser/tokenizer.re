#include "grammar.h"
#include "tokenizer.h"

#define FILL_LEX(q) YYCURSOR = q

char *YYCURSOR;

static int lex(void)
{
	char *YYMARKER, *o1;

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

		int @o1 { return symbol_table_append(INT, o1); }
		float @o1 { return symbol_table_append(FLOAT, o1); }
		letter @o1 { return symbol_table_append(LETTER, o1); }

	// Arithmetic Operator and Misc. Defs.

		"+" @o1 { return symbol_table_append(PLUS, o1); }
		"-" @o1 { return symbol_table_append(MINUS, o1);  }
		"*" @o1 { return symbol_table_append(MULT, o1); }
		"/" @o1 { return symbol_table_append(DIV, o1); }
		"(" @o1 { return symbol_table_append(LPAREN, o1); }
		")" @o1 { return symbol_table_append(RPAREN, o1);}

	// System Defs.

		"\x00" @o1 { return symbol_table_append(EOL, o1); }
		* { return UNKNOWN; }

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
	char *addr = query;

	clear_ersl(ersl);
	symbol_table_clear();

	FILL_LEX(query);

	symbol_table_init(query);

	printf("token \t| addr \t\t| string \t| float\n");
	printf("rsv \t| %p \t| NULL \t\t|NaN\n", query);
	while (lex() != EOL) {
	}
	parse(parser, 0, 0, ersl);
end:
	parse_free(parser, free);
}
