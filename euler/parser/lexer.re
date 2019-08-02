#include "parser.h"
#include "lexer.h"

token_val tknv;

const char *YYCURSOR;

static int lex(void)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat="
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

	const char *YYMARKER, *o1, *o2;

	/* reset token table */
	tknv.val = 0;
	*tknv.name = '\x00';

	// Settings
	/*!stags:re2c format = 'const char *@@;'; */
	/*!re2c
		re2c:define:YYCTYPE = char;
		re2c:yyfill:enable = 0;


	// Expression Defs.

		int = [0-9]+;
		float = [0-9]+('.'[0-9]+)('e'[+-]?[0-9]+)? ;
		var = [A-Z]+;


	// Number Defs.

		@o1 int @o2 {
			sprintf(tknv.name, "%.*s",(int)(o2 - o1), o1);
			tknv.val = atof(tknv.name);
			return INT;
		}

		@o1 float @o2 {
			sprintf(tknv.name, "%.*s",(int)(o2 - o1), o1);
			tknv.val = atof(tknv.name);
			return FLOAT;
		}
		@o1 var @o2{
			sprintf(tknv.name, "%.*s",(int)(o2 - o1), o1);
			return VAR;
		}


	// Arithmetic Operator and Misc. Defs.

		"+" { return PLUS; }
		"-" { return MINUS;  }
		"*" { return MULT; }
		"/" { return DIV; }
		"=" { return EQ; }
		"(" { return LPAREN;}
		")" {return RPAREN;}
		"," { return COMMA; }




	// Trig Defs.

		"cos" {return COS;}
		"acos" {return ACOS;}
		"cosh" {return COSH;}

		"sin" {return SIN;}
		"asin" {return ASIN;}
		"sinh" {return SINH;}

		"tan" {return TAN;}
		"atan" {return ATAN;}
		"tanh" {return TANH;}

		"cot" {return COT;}


	// Basic Math Defs.

		"exp" {return EXP;}
		"log" {return LOG;}
		"ln" {return LN;}

		"pow" {return POW;}
		"sqrt" {return SQRT;}
		"cbrt" {return CBRT;}

		"ceil" {return CEIL;}
		"floor" {return FLOOR;}
		"mod" {return MOD;}

		"abs" {return ABS;}


	// Constant Defs.

		"pi" {
			tknv.val = M_PI;
			return CONST;
		}

	// Calculus Defs.

		"integrate" {
			return INTEGRATE;
		}
		"from" {
			return FROM;
		}
		"to" {
			return TO;
		}

	// System Defs.

		"list" {return LIST; }
		"quit" {return QUIT;}
		*   { return UNKNOWN; }
		"\x00" {return EOL;}
	*/

#pragma GCC diagnostic pop
}

int parse_query(char *query, ersl_t *ersl)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"

	int token;
	void *parser = parse_alloc(malloc);

	YYCURSOR = query;

	ersl->error = 0; /* reset error flag */

	while ((token = lex()) != EOL) {
		parse(parser, token, tknv, ersl);
	}
	parse(parser, 0, tknv, ersl);

	parse_free(parser, free);

	return ersl->error;
#pragma GCC diagnostic pop
}
