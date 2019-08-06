
#ifndef __LEXER__
#define __LEXER__
#include "../inc/euler.h"
/* Just change of notation */
#define parse Parse
#define parse_free ParseFree
#define parse_alloc ParseAlloc

/* Terminal symbols */
#define EOL 0 /* End of the query */
#define UNKNOWN -2

typedef union {
	char name[10]; /* TODO: Connect this length to variables.h. This length defined in variables.h */
	double val;
} token_val;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
void *ParseAlloc(void *(*mallocProc)(YYMALLOCARGTYPE));
void Parse(void *yyp, /* The parser */
	   int yymajor, /* The major token code number */
	   token_val yyminor, /* The value for the token */
	   ersl_t *euler /* Optional %extra_argument parameter */
);
#pragma GCC diagnostic pop

void free_raw_after_equal(void);
char *get_raw_after_equal(void);

#endif /* __LEXER__ */
