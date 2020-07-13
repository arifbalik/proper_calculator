#ifndef _AST_
#define _AST_
#include "../inc/euler.h"

/* construct macros */
#define NODE(t, l, r) ast_add_node(t, l, r)
#define LEAF_C(t, v) ast_add_leaf_const(t, v)

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
#define ISNUMERIC(u) (KIND(u, INT) || KIND(u, FLOAT) || ISNEGATIVE(u))
#define ISLITERAL(u) (KIND(u, LETTER))
#define ISNEGATIVE(u) (KIND(u, MINUS) && !(u->left) && u->right)
/* Special function macros */
#define NOPS(u)                                                                \
	((LEFT(u) == NULL && RIGHT(u) == NULL) ?                               \
		 0 :                                                           \
		 (LEFT(u) && RIGHT(u)) ? 2 : 1)
#define NVAL(u) ((ISNEGATIVE(u)) ? -(RIGHT(u)->value.number) : u->value.number)

#define DENOMINATOR(u) ((u->right == NULL) ? 1 : NVAL(ast_operand(u, 2)))
#define NUMERATOR(u) ((u->left == NULL) ? NVAL(u) : NVAL(ast_operand(u, 1)))

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
ast_t *ast_operand(ast_t *u, int16_t i);

#endif /* _AST_ */
