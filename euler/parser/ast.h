#ifndef _AST_
#define _AST_
#include "../inc/euler.h"

#ifdef UNIX
void ast_print(ast_t *tree);
#endif

void ast_clear(ersl_t *euler);
ast_t *ast_add_leaf(ersl_t *euler, uint8_t type);
ast_t *ast_add_leaf_const(ersl_t *euler, uint8_t type, double value);
ast_t *ast_add_node(ersl_t *euler, uint8_t type, ast_t *ast_left,
		    ast_t *ast_right);

#endif /* _AST_ */
