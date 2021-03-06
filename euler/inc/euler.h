#ifndef _EULER_
#define _EULER_

#include <math.h>
#include <stdint.h>

#define MAX_QUERY_LENGTH 80
#define MAX_AST_BRANCH 100

#define _TEST_

#define UNIX

#ifdef UNIX
#include <stdio.h>
#include <stdlib.h>
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#define DOUBLE_PRECISION 10

#ifdef __GNUC__
#define __PACK __attribute__((packed))
#else
#error "Please use GCC to compile euler!"
#endif

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
} __PACK ast_t;

/* Symbol Table Structre */
typedef struct {
	/* This holds the beggininng of matched token.
	 * Look for next index to get the end
	 */
	char *p;

	uint8_t no; /* Hold token number, defined in grammar.h */
	uint8_t priority; /* For partial parsing */

} __PACK token_t;

typedef struct {
	token_t token[MAX_QUERY_LENGTH];
	uint8_t top; /* index of last element in the stack */
	uint8_t cur; /* currently handled token index */

} __PACK symbol_table_t;

typedef struct {
	estatus_t status; /* Developer can use this to debug the engine */
	char *ascii; /* The query */
	double (*func)(void);
	symbol_table_t symbol_table; /* Token table */
	ast_t *ast[MAX_AST_BRANCH]; /* Abstract Syntax Tree */
	uint8_t ast_top_idx; /* index of top node */

#ifdef _TEST_
	char out[255];
	uint8_t out_pos;
#endif

	/* Reserved Abstract Syntax Tree.
	 * Only used by euler in custom malloc operations
	 */
	ast_t ast_rsv[MAX_AST_BRANCH];

} __PACK ersl_t;

void parse_query(void);
void euler_init(void);
ersl_t *_euler(void);

#endif /* _EULER_ */
