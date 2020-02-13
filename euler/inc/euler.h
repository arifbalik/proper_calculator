#ifndef _EULER_
#define _EULER_

#include <math.h>
#include <stdint.h>

#define MAX_QUERY_LENGTH 80
#define MAX_AST_BRANCH 100

#define UNIX

#ifdef UNIX
#include <stdio.h>
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#define DOUBLE_PRECISION 10

/* Error defs. */
typedef enum {
	NONE = 255,

	/* Those are system spesific events.
	 * Usually lexer uses them.
	 */
	_PDONE, /* Parse Done */
	_PAGAIN, /* Parse Again */

	_FEX, /* Function Expand */

	/* Euler std. errors */
	MTHE = -128, /* Math Error */
	EMPTYQ, /* Empty Query */
	SYMNF, /* Symbol Not Found */

	/* Variable errors */
	MXVQ, /* Max Variable Quantity Has Been Reaced */
	NVF, /* No Variable Found */

	/* Function errors */
	FNF, /* Function Not Found */
	CNEF, /* Can Not Evaluate Function */
	UNDEFR /* Undefined Eesult Type */
} estatus_t;

/* AST Structre */
typedef struct ast {
	uint8_t type; /* This is the token value in grammar.h */

	/* Value of the token */
	union {
		double number;
		char literal;
	} value;

	struct ast *left; /* left hand side of the node */
	struct ast *right; /* right hand side of the node */
} __attribute__((packed)) ast_t;

/* Symbol Table Structre */
typedef struct {
	/* This holds the beggininng of matched token.
	 * Look for next index to get the end
	 */
	char *p;

	uint8_t no; /* Hold token number, defined in grammar.h */
	uint8_t priority; /* For partial parsing */

} __attribute__((packed)) token_t;

typedef struct {
	token_t token[MAX_QUERY_LENGTH];
	uint8_t top; /* index of last element in the stack */
	uint8_t cur; /* currently handled token index */

} __attribute__((packed)) symbol_table_t;

typedef struct {
	estatus_t status; /* Developer can use this to debug the engine */
	char *ascii; /* The query */
	double (*func)(void);
	symbol_table_t symbol_table; /* Token table */
	ast_t *ast[MAX_AST_BRANCH]; /* Abstract Syntax Tree */
	uint8_t ast_top_idx; /* index of top node */

	/* Reserved Abstract Syntax Tree.
	 * Only used by euler in custom malloc operations
	 */
	ast_t ast_rsv[MAX_AST_BRANCH];

} __attribute__((packed)) ersl_t;

void parse_query(ersl_t *ersl);
void euler_init(void);

#endif /* _EULER_ */
