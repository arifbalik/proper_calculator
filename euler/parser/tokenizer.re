#include "grammar.h"
#include "tokenizer.h"

#define FILL_LEX(q) YYCURSOR = q

char *YYCURSOR;

static int lex(void)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat="
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

	char *YYMARKER, *o1;

	/* reset token table */
	tknv->val = 0;
	*tknv->name = '\x00';

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

		@o1 int { return symbol_table_append(INT, o1); }
		@o1 float { return symbol_table_append(FLOAT, o1); }
		@o1 letter { return symbol_table_append(LETTER, o1); }

	// Arithmetic Operator and Misc. Defs.

		@o1 "+" { return symbol_table_append(PLUS, o1); }
		@o1 "-" { return symbol_table_append(MINUS, o1);  }
		@o1 "*" { return symbol_table_append(MULT, o1); }
		@o1 "/" { return symbol_table_append(DIV, o1); }
		@o1 "=" { return symbol_table_append(EQ, o1); }
		@o1 "(" { return symbol_table_append(LPAREN, o1); }
		@o1 ")" { return symbol_table_append(RPAREN, o1);}
		@o1 "," { return symbol_table_append(COMMA, o1); }

	// Trig Defs.

		@o1 "cos" { return symbol_table_append(COS, o1); }
		@o1 "acos" { return symbol_table_append(ACOS, o1); }
		@o1 "cosh" { return symbol_table_append(COSH, o1); }
		@o1 "sin" { return symbol_table_append(SIN, o1); }
		@o1 "asin" { return symbol_table_append(ASIN, o1); }
		@o1 "sinh" { return symbol_table_append(SINH, o1); }
		@o1 "tan" { return symbol_table_append(TAN, o1); }
		@o1 "atan" { return symbol_table_append(ATAN, o1); }
		@o1 "tanh" { return symbol_table_append(TANH, o1); }
		@o1 "cot" { return symbol_table_append(COT, o1); }

	// Basic Math Defs.

		@o1 "exp" { return symbol_table_append(EXP, o1); }
		@o1 "log" { return symbol_table_append(LOG, o1); }
		@o1 "ln" { return symbol_table_append(LN, o1); }
		@o1 "pow" { return symbol_table_append(POW, o1); }
		@o1 "sqrt" { return symbol_table_append(SQRT, o1); }
		@o1 "cbrt" { return symbol_table_append(CBRT, o1); }
		@o1 "ceil" { return symbol_table_append(CEIL, o1); }
		@o1 "floor" { return symbol_table_append(FLOOR, o1); }
		@o1 "mod" { return symbol_table_append(MOD, o1); }
		@o1 "abs" { return symbol_table_append(ABS, o1); }

	// Constant Defs.

		@o1 "pi" { return symbol_table_append(CONST, o1); }

	// Calculus Defs.

		@o1 "integrate" { return symbol_table_append(INTEGRATE, o1); }
		@o1 "from" { return symbol_table_append(FROM, o1); }
		@o1 "to" { return symbol_table_append(TO, o1); }

	// User function
		@o1 "eval" { return symbol_table_append(EVAL, o1); }
		@o1 "steps" { return symbol_table_append(STEPS, o1); }

	// System Defs.

		@o1 "list" { return symbol_table_append(LIST, o1); }
		@o1 "quit" { return symbol_table_append(QUIT, o1); }
		@o1 "\x00" { return symbol_table_append(EOL, o1); }
		* { return UNKNOWN; }

	*/
#pragma GCC diagnostic pop
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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"

	void *parser = parse_alloc(malloc);
	char *addr = query;

	clear_ersl(ersl);
	symbol_table_clear();

	FILL_LEX(query);

	while (1) {
		lex();
		//parse(parser, symbol_table[table_index].token, symbol_table,
		//    ersl);
	}
end:
	parse_free(parser, free);

#pragma GCC diagnostic pop
}
