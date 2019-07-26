#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "parser.h"
#include "includes.h"
#include "../euler/inc/variables.h"
#include "../arch/arm/stm32f746ng/print.h"

#define GETSV(bstr, estr) strndup(estr, bstr - estr)
#define GETFV(bstr, estr) atof(GETSV(bstr, estr))
#define GETIV(bstr, estr) atoi(GETSV(bstr, estr))

float stof(const char *s);
int lex(char *equery);

const char *YYCURSOR;
const char *exsp;
token_info tkn;

static int fch = 0; /* flag to indicate first variable name in the query line */

int lex(char *equery)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat="
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

	const char *YYMARKER, *o1, *o2;

	if (YYCURSOR >= equery) {
		return EOL;
	}
/*!stags:re2c format = 'const char *@@;'; */
/*!re2c
	re2c:define:YYCTYPE = char;
	re2c:yyfill:enable = 0;

	int = [0-9]+;
	float = [0-9]+('.'[0-9]+)('e'[+-]?[0-9]+)? ;
	var = [A-Z]+;
	*/

/*!re2c
	@o1 int @o2 {


		sprintf(tkn.name, "%.*s",(int)(o2 - o1));
		tkn.val = stof(tkn.name);
            return INT;
        }
	@o1 float @o2 {
		sprintf(tkn.name, "%.*s",(int)(o2 - o1));
		tkn.val = stof(tkn.name);
            return FLOAT;
        }
	@o1 var @o2{
                sprintf(tkn.name, "%.*s",(int)(o2 - o1));
		if(fch == 0){
			fch = 1;
			strcpy(fvar, tkn.name);
		}
		return VAR;
	}




	"+" { return PLUS; }
	"-" { return MINUS;  }
	"*" { return MULT; }
	"/" { return DIV; }
	"=" { return EQ; }
	"(" { return LPAREN;}
	")" {return RPAREN;}
        "list" {return LIST; }
	"quit" {return QUIT;}
	*   { return QUIT; }


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

	"," { return COMMA; }



	"pi" {return PI;}


	*/
#pragma GCC diagnostic pop
}

float stof(const char *s)
{
	float rez = 0, fact = 1;
	if (*s == '-') {
		s++;
		fact = -1;
	}
	for (int point_seen = 0; *s; s++) {
		if (*s == '.') {
			point_seen = 1;
			continue;
		}
		int d = *s - '0';
		if (d >= 0 && d <= 9) {
			if (point_seen)
				fact /= 10.0f;
			rez = rez * 10.0f + (float)d;
		}
	}
	return rez * fact;
}

int cmd(char *query)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
	char *equery;
	void *parser;
	int token = -3;
	char tk[30];
	char *tmp;
	float no;

	parser = (void *)ParseAlloc(malloc);

	equery = (char *)(((char *)query) + strlen(query));
	YYCURSOR = exsp = query;

	while (token != EOL) {
		token = lex(equery);

		//console_puts(tk);
		Parse(parser, token, tkn);
		exsp = YYCURSOR;
	}
	fch = 0;
	tkn.val = 0;
	strcpy(tkn.name, " ");
	token = -3;

	ParseFree(parser, free);
	return (0);
#pragma GCC diagnostic pop
}
