#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "euler.h"
#include "variables.h"

#define GETSV(bstr, estr) strndup(estr, bstr - estr)
#define GETFV(bstr, estr) atof(GETSV(bstr, estr))
#define GETIV(bstr, estr) atoi(GETSV(bstr, estr))

static const char *YYCURSOR;
const char *exsp;
struct token_info *tkn;

static int fch = 0; /* flag to indicate first variable name in the query line */

int lex(char *equery)
{
	const char *YYMARKER;

	if (YYCURSOR >= equery) {
		return EOL;
	}
	/*!re2c
	re2c:define:YYCTYPE = char;
	re2c:yyfill:enable = 0;

	int = [-+]?[0-9]+;
	float = [-+]?[0-9]+('.'[0-9]+)('e'[+-]?[0-9]+)? ;
	name = [A-Z]+;
	*/

	/*!re2c
	int {
		tkn->val = GETIV(YYCURSOR, exsp);
                return INT;
	}
	float {
		tkn->val = GETFV(YYCURSOR, exsp);
                return FLOAT;
	}
	name {
                strcpy(tkn->name,GETSV(YYCURSOR, exsp));
		if(fch == 0){
			fch = 1;
			strcpy(fvar, tkn->name);
			printf("fvar saved!\n");
		}
		return NAME;
	}


	"p" { return PLUS; }
	"m" { return MINUS;  }
	"*" { return MULT; }
	"/" { return DIV; }
	"=" { return EQ; }
        "list" {return LIST; }
	"quit" {return QUIT;}
	*   { return QUIT; }
	*/
}

int main(int argc, char **argv)
{
	char *query, *equery;
	void *parser;
	int token;

	query = (char *)malloc(MAX_QUERY_SIZE * sizeof(char));
	tkn = malloc(sizeof(struct token_info));
	tkn->name = (char *)malloc(sizeof(char) * MAX_VARNAME_LEN);
	parser = ParseAlloc(malloc);

	while (1) {
		printf(">>>");
		scanf("%s", query);
		equery = (char *)(((char *)query) + strlen(query));
		YYCURSOR = exsp = query;

		while (token != EOL) {
			token = lex(equery);
			if (token == QUIT)
				goto quit;
			Parse(parser, token, *tkn);
			exsp = YYCURSOR;
		}
		fch = 0;
		tkn->val = 0;
		strcpy(tkn->name, " ");
		token = -3;
	}
quit:
	free(tkn->name);
	free(tkn);
	ParseFree(parser, free);
	free(query);
	return (0);
}
