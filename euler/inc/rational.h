#ifndef __RATIONAL__
#define __RATIONAL__

ast_t *simplify_rational_number(ast_t *u);
ast_t *simplify_rne(ast_t *u);
ast_t *eval_sum(ast_t *u, ast_t *v);

#endif /* __RATIONAL */
