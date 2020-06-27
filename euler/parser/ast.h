#ifndef _AST_
#define _AST_
#include "../inc/euler.h"

#define ISNODE(node) !ISLEAF(node)
#define ISLEAF(leaf) (leaf->left == NULL && leaf->right == NULL)
#define IS_NULL(node) ((node != NULL) ? node : NULL)

#define LEAF_NUMBER_VAL(leaf, val) (leaf->value.number == val)
#define LEAF_LITERAL_VAL(leaf, val) (leaf->value.literal == val)

#define LEFT(n) n->left
#define RIGHT(n) n->right

#define KIND(u, v) (u->type == v)
#define ISOPERATOR(u)                                                          \
	(KIND(u, PLUS) || KIND(u, MINUS) || KIND(u, MULT) || KIND(u, DIV) ||   \
	 KIND(u, EQ))
#define ISOPERAND(u) (!ISOPERATOR(u))
#define GET_LEVEL(n) (log((double)n) / log(2))
#define GET_POS(n) (n - pow(2, GET_LEVEL(n)))
#define ISNUMERIC(u) (KIND(u, INT) || KIND(u, FLOAT))
#define ISLITERAL(u) (KIND(u, LETTER))

#ifdef UNIX
void ast_print(ast_t *root);
#endif

void ast_init();
void ast_finalize();
void ast_relink_node(ast_t *child, ast_t *new_child);
uint8_t ast_get_available_slots();
ast_t *ast_destroy_node(ast_t *node);
ast_t *ast_get_root();
uint8_t ast_get_root_idx();
ast_t **ast_find_parent(ast_t *child);
ast_t *ast_add_leaf(uint8_t type);
ast_t *ast_add_leaf_const(uint8_t type, double value);
ast_t *ast_add_leaf_literal(uint8_t type, char value);
ast_t *ast_add_node(uint8_t type, ast_t *ast_left, ast_t *ast_right);
void ast_node_copy(ast_t *dest, ast_t *src);
uint8_t ast_comp_val(ast_t *n1, ast_t *n2);
ast_t *ast_malloc();
void ast_operand(ast_t *u, int16_t i, ast_t *operand);

#endif /* _AST_ */
