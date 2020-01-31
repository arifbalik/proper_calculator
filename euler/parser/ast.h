#ifndef _AST_
#define _AST_
#include "../inc/euler.h"

#define AST_ISNODE(node) !AST_ISLEAF(node)
#define AST_ISLEAF(leaf) (leaf->left == NULL && leaf->right == NULL)
#define AST_NULL_CHECK(node) ((node != NULL) ? node : NULL)

#define AST_TYPE_CHECK(node, t) (node->type == t)
#define AST_TYPE_CHECK_2(node, t1, t2)                                         \
	((node->type == t1) || (node->type == t2))
#define AST_TYPE_CHECK_3(node, t1, t2, t3)                                     \
	((node->type == t1) || (node->type == t2) || (node->type == t3))

#define AST_WHICH_CHILD_HAS_TYPE(node, t)                                      \
	((AST_TYPE_CHECK(node->left, t)) ?                                     \
		 node->left :                                                  \
		 ((AST_TYPE_CHECK(node->right, t) ? node->right : NULL)))

#define AST_WHICH_CHILD_HAS_TYPE_2(node, t1, t2)                               \
	((AST_TYPE_CHECK_2(node->left, t1, t2)) ?                              \
		 node->left :                                                  \
		 ((AST_TYPE_CHECK_2(node->right, t1, t2) ? node->right :       \
							   NULL)))
#define AST_OTHER_CHILD(node, child)                                           \
	((node->left == child) ? node->right :                                 \
				 (node->right == child) ? node->left : NULL)

#define AST_WHICH_CHILD_HAS_TYPE_3(node, t1, t2, t3)                           \
	((AST_TYPE_CHECK_3(node->left, t1, t2, t3)) ?                          \
		 node->left :                                                  \
		 ((AST_TYPE_CHECK_3(node->right, t1, t2, t3) ? node->right :   \
							       NULL)))

#define AST_TYPE_CHECK_CHILDS(node, t)                                         \
	(AST_TYPE_CHECK(node->left, t) ?                                       \
		 node->left :                                                  \
		 AST_TYPE_CHECK(node->right, t) ? node->right : NULL)

#define AST_VALUE_CHECK_CHILDS(node, t, postfix, val)                          \
	(((node->left->type == t) && (node->left->value.postfix == val)) ?     \
		 node->left :                                                  \
		 ((node->right->type == t) &&                                  \
		  (node->right->value.postfix == val)) ?                       \
		 node->right :                                                 \
		 NULL)

#define AST_VALUE_CHECK_CHILDS_2(node, t1, t2, postfix, val)                   \
	((node->left != NULL &&                                                \
	  (node->left->type == t1 || node->left->type == t2) &&                \
	  (node->left->value.postfix == val)) ?                                \
		 node->left :                                                  \
		 (node->right != NULL &&                                       \
		  (node->right->type == t1 || node->right->type == t2) &&      \
		  (node->right->value.postfix == val)) ?                       \
		 node->right :                                                 \
		 NULL)

#define AST_LEAF_NUMBER_VAL(leaf, val) (leaf->value.number == val)

#define AST_LEAF_LITERAL_VAL(leaf, val) (leaf->value.literal == val)

#ifdef UNIX
void ast_print(ast_t *root);
#endif

void ast_init(ersl_t *euler);
void ast_finalize(ersl_t *euler);
void ast_relink_node(ersl_t *euler, ast_t *child, ast_t *new_child);
uint8_t ast_get_available_slots(ersl_t *euler);
ast_t *ast_destroy_node(ersl_t *euler, ast_t *node);
ast_t *ast_get_root(ersl_t *euler);
uint8_t ast_get_root_idx(ersl_t *euler);
ast_t **ast_find_parent(ersl_t *euler, ast_t *child);
ast_t *ast_add_leaf(ersl_t *euler, uint8_t type);
ast_t *ast_add_leaf_const(ersl_t *euler, uint8_t type, double value);
ast_t *ast_add_leaf_literal(ersl_t *euler, uint8_t type, char value);
ast_t *ast_add_node(ersl_t *euler, uint8_t type, ast_t *ast_left,
		    ast_t *ast_right);

#endif /* _AST_ */
