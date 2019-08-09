#include "parser.h"
#include "lexer.h"

#define BACKUPQ YYCURSOR = addr
#define FILL_LEX(q) YYCURSOR = q

char *raw_after_equal; /* to hold the raw input after equal. Nedded for functions */

void free_raw_after_equal(void)
{
	free(raw_after_equal);
}
char *get_raw_after_equal(void)
{
	return raw_after_equal;
}

char *YYCURSOR, *o1, *o2, *o3, *o4;

static int lex(token_val *tknv)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat="
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

	char *YYMARKER;

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

		@o1 int @o2 {
			sprintf(tknv->name, "%.*s",(int)(o2 - o1), o1);
			tknv->val = atof(tknv->name);
			return INT;
		}

		@o1 float @o2 {
			sprintf(tknv->name, "%.*s",(int)(o2 - o1), o1);
			tknv->val = atof(tknv->name);
			return FLOAT;
		}
		@o1 letter @o2{
			sprintf(tknv->name, "%.*s",(int)(o2 - o1), o1);
			return LETTER;
		}


	// Arithmetic Operator and Misc. Defs.

		"+" { return PLUS; }
		"-" { return MINUS;  }
		"*" { return MULT; }
		"/" { return DIV; }
		"=" {
			raw_after_equal = (char *)malloc(sizeof(char) * strlen(YYCURSOR) + 1);
			raw_after_equal[strlen(YYCURSOR)] = '\0';
			strncpy(raw_after_equal, YYCURSOR, strlen(YYCURSOR));
			return EQ;
		}
		"(" { return LPAREN; }
		")" {return RPAREN;}
		"," { return COMMA; }

		* { return UNKNOWN; }



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
			tknv->val = M_PI;
			return CONST;
		}

	// Calculus Defs.

		"integrate" {
			return INTEGRATE;
		}
		("from") {
			return FROM;
		}
		("to") {
			return TO;
		}

	// User function
		("eval") { return EVAL; }
		("steps") { return STEPS; }


	// Function placement

		@o1 (letter) @o2 '('  [^a-z]+ ')'@o3   {
			return FN_CALL;
		}

	// System Defs.

		"list" {return LIST; }
		"quit" {return QUIT;}
		"\x00" {return EOL;}

	*/
#pragma GCC diagnostic pop
}

// static void pfncpr(const char *query)
// {
// 	char *tmp; /* we'll use it for lexer and store the result in original query var. */
// 	token_val dummy; /* just for function call */
//
// 	/* TODO: determine how much space tmp should allocate */
// 	tmp = (char *)malloc(sizeof(char) * strlen(query) * 4);
//
// 	strcpy(tmp, query);
// 	YYCURSOR = tmp;
//
// 	place_flag =
// 		1; /* when we call lex with this flag set to 1 it search for functions */
// 	while (lex(&dummy, query) != EOL) {
// 		/* There might be new function to be pre-parsed.
// 		 * So we call it again too find out.
// 		 */
// 		if (place_flag == 2) {
// 			strcpy(tmp, query); /* fill tmp with pre-parsed query */
// 			YYCURSOR = tmp;
// 		}
// 		place_flag = 1;
// 	}
// 	place_flag = 0;
// 	YYCURSOR = query; /* store the final result */
// }

static void free_ersl(ersl_t *ersl)
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

	uint8_t token;
	token_val tknv;
	void *parser = parse_alloc(malloc);
	char *addr = query;

	free_ersl(ersl);
	//pfncpr(query);
	FILL_LEX(query);
	while (1) {
		token = lex(&tknv);

		switch (token) {
		case FN_CALL: /* function expand */
			BACKUPQ;
			ersl->status = fnexp(o1, o2, o3, YYCURSOR);
			if (ersl->status < 0)
				goto end;
			parser_restart(parser);
			continue;
		case EOL:
			parse(parser, 0, tknv, ersl);
			goto end;
		default:
			break;
		}
		parse(parser, token, tknv, ersl);
	}
end:
	parse_free(parser, free);

#pragma GCC diagnostic pop
}
