#ifndef _AST_
#define _AST_

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
} ast_t;

ast_t *ast_add_leaf(ast_t *ast, uint8_t type, void *value);
ast_t *add_node(ast_t *ast_node, uint8_t type, void *value, ast_t *ast_left,
		ast_t *ast_right);

#endif /* _AST_ */
