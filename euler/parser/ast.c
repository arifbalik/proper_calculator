#include "../inc/euler.h"
#include "ast.h"
#include "grammar.h"
#include "symbol_table.h"

#define KIND(u, v) (u->type == v)
#define ISOPERATOR(u)                                                          \
	(KIND(u, PLUS) || KIND(u, MINUS) || KIND(u, MULT) || KIND(u, DIV) ||   \
	 KIND(u, EQ))
#define ISOPERAND(u) (!ISOPERATOR(u))
#define GET_LEVEL(n) (log((double)n) / log(2))
#define GET_POS(n) (n - pow(2, GET_LEVEL(n)))
#define ISNUMERIC(u) (KIND(u, INT) || KIND(u, FLOAT))
#define ISLITERAL(u) (KIND(u, LETTER))

void ast_node_copy(ast_t *dest, ast_t *src);
uint8_t ast_comp_val(ast_t *n1, ast_t *n2);
static ast_t *ast_malloc(ersl_t *euler);
void ast_operand(ast_t *u, int16_t i, ast_t *operand);

/***** TEST ****/
int integer_gcd(int n, int d)
{
	int r;

	while (d != 0) {
		r = n % d;
		n = d;
		d = r;
	}
	return abs(n);
}


/*** END TEST ****/

/*! Primitive Structural Operators. */

void _get_operand(ast_t *u, uint8_t level, uint8_t pos, int16_t *_tmp,
		  ast_t *operand)
{
	if (u == NULL)
		return;
	if (level == 1) {
		if (pos == *_tmp)
			ast_node_copy(operand, u);
	}

	else if (level > 1) {
		*_tmp += 1;
		_get_operand(u->left, level - 1, pos, _tmp, operand);
		*_tmp += 1;
		_get_operand(u->right, level - 1, pos, _tmp, operand);
	}

	return;
}

/*!
 * returns the nth operand in the AST (level ordered search)
 */
void ast_operand(ast_t *u, int16_t i, ast_t *operand)
{
	int16_t _tmp_pos = -1;

	if (i == 1)
		ast_node_copy(operand, u->right);
	else if (i == 2)
		ast_node_copy(operand, u->left);
	else
		_get_operand(u, GET_LEVEL(i) + 1, GET_POS(i), &_tmp_pos,
			     operand);
}

/*! Structural Operations */

/*!
 * searches two trees and returns 1 if they are identical 0 otherwise
 */
uint8_t ast_isidentical(ast_t *u, ast_t *t)
{
	if (u == NULL && t == NULL)
		return 1;

	if (u == NULL || t == NULL)
		return 0;

	/* Check if the data of both roots is same and data of left and right 
       subtrees are also same */
	return (ast_comp_val(u, t) && ast_isidentical(u->left, t->left) &&
		ast_isidentical(u->right, t->right));
}

/*!
 * if an AST is not included (free of) another tree function returns 1, 0 otherwise
 */
uint8_t ast_free_of(ast_t *u, ast_t *t)
{
	/* base cases */
	if (t == NULL)
		return 0;

	if (u == NULL)
		return 1;

	/* Check the tree with root as current node */
	if (ast_isidentical(u, t))
		return 0;

	/* If the tree with root as current node doesn't match then 
       try left and right subtrees one by one */
	return ast_free_of(u->left, t) && ast_free_of(u->right, t);
}

/*!
 * substitutes a subtree with a single leaf node.
 */
void ast_substitute(ersl_t *euler, ast_t *u, ast_t *t, ast_t *r)
{
	if (!r || !u || !t || !euler)
		return;
	if (r->left || r->right) /* only single variable substutution */
		return;

	if (ast_isidentical(u, t)) {
		printf("identical\n");
		ast_relink_node(euler, u, r);
	}

	ast_substitute(euler, u->left, t, r);
	ast_substitute(euler, u->right, t, r);
}

void ast_init(ersl_t *euler)
{
	uint8_t idx = 0;

	/* Load the addresses of ast_rsv to ast and initialize members */
	while (idx < MAX_AST_BRANCH) {
		euler->ast[idx] = &(euler->ast_rsv[idx]);
		euler->ast[idx]->type = 0;
		euler->ast[idx]->left = NULL;
		euler->ast[idx]->right = NULL;
		idx++;
	}
	euler->ast_top_idx = 0;
}

int ast_to_infix(ast_t *root, int total)
{
	if (root == NULL)
		return total;

	ast_to_infix(root->left, total);
	if (root->left == NULL && root->right == NULL) {
		if (root->type == 2) {
			printf("%f", root->value.number);
			total += root->value.number;
		}
	} else {
		printf("(%d)", root->type);
	}

	ast_to_infix(root->right, total);

	return total;
}

void ast_finalize(ersl_t *euler)
{
	euler->ast_top_idx = ast_get_root_idx(euler);
	ast_t *tmp = simplify_rational_number(euler, ast_get_root(euler));
	if (tmp != NULL)
		ast_print(tmp);
}

/* return the address of the next available element in erstl_t */
static ast_t *ast_malloc(ersl_t *euler)
{
	uint8_t idx = 0;

	/* Check if the type is 0
	 * which means its available cause 0 is reserved
	 */
	while (euler->ast[idx] != NULL && euler->ast[idx]->type != 0) {
		idx++;
		if (idx >= MAX_AST_BRANCH)
			return NULL; /* Array is full */
	}

	return euler->ast[idx];
}

static void ast_write_value(ersl_t *euler, ast_t *ast)
{
	if (ast == NULL)
		return;
	switch (ast->type) {
	case INT:
	case FLOAT:
		ast->value.number = st_get_number(&(euler->symbol_table));
		break;
	case LETTER:
		ast->value.literal = st_get_letter(&(euler->symbol_table));
		break;
	default:
		return; /* Unsupported type */
		break;
	}
}

/* Find the parent of a given leaf or node by searching the entire ast. */
ast_t **ast_find_parent(ersl_t *euler, ast_t *child)
{
	uint8_t idx = 0;

	while (idx < MAX_AST_BRANCH) {
		if (euler->ast[idx] != NULL && AST_ISNODE(euler->ast[idx]) &&
		    (euler->ast[idx]->left == child ||
		     euler->ast[idx]->right == child)) {
			return &(euler->ast[idx]);
		}
		idx++;
	}

	return NULL;
}

ast_t *ast_destroy_node(ersl_t *euler, ast_t *node)
{
	ast_t **parent = ast_find_parent(euler, node);
	ast_t *tmp = NULL;
	if (parent != NULL) {
		tmp = ((*parent)->left == node) ? (*parent)->right :
						  (*parent)->left;
		(*parent)->type = tmp->type;
		(*parent)->value = tmp->value;
		(*parent)->left = (*parent)->right = NULL;
		if (AST_ISNODE(tmp)) {
			(*parent)->left = tmp->left;
			(*parent)->right = tmp->right;
		}
		printf("destroyed type %d, r %p l %p\n", (*parent)->type,
		       (*parent)->right, (*parent)->left);
	}
	return *parent;
}
void ast_relink_node(ersl_t *euler, ast_t *child, ast_t *new_child)
{
	ast_t **parent = NULL;
	ast_t *root = ast_get_root(euler);

	if (child == root) {
		euler->ast[euler->ast_top_idx] = new_child;
		return;
	}

	/* Find parent(s) of the child. When a child is referenced to a new
	 * parent and that child is destroyed, than it should be found and
	 * replaced with new_child as well.
	 */
	while ((parent = ast_find_parent(euler, child)) != NULL) {
		if ((*parent)->left == child)
			(*parent)->left = new_child;
		else
			(*parent)->right = new_child;
	}
}

ast_t *ast_get_root(ersl_t *euler)
{
	return euler->ast[euler->ast_top_idx];
}

uint8_t ast_get_root_idx(ersl_t *euler)
{
	uint8_t idx = 0;

	while (euler->ast[idx]->type != 0) {
		idx++;
		if (idx >= MAX_AST_BRANCH) {
			return MAX_AST_BRANCH;
		}
	}

	return idx - 1;
}

ast_t *ast_add_leaf(ersl_t *euler, uint8_t type)
{
	ast_t *ast = ast_malloc(euler);

	if (ast == NULL)
		return NULL;

	ast->type = type;

	ast_write_value(euler, ast);

	ast->left = NULL;
	ast->right = NULL;

	return ast;
}

ast_t *ast_add_leaf_const(ersl_t *euler, uint8_t type, double value)
{
	ast_t *ast = ast_malloc(euler);

	if (ast == NULL)
		return NULL;

	ast->type = type;

	ast->value.number = value;

	ast->left = NULL;
	ast->right = NULL;

	return ast;
}

ast_t *ast_add_leaf_literal(ersl_t *euler, uint8_t type, char value)
{
	ast_t *ast = ast_malloc(euler);

	if (ast == NULL)
		return NULL;

	ast->type = type;

	ast->value.literal = value;

	ast->left = NULL;
	ast->right = NULL;

	return ast;
}
uint8_t ast_get_available_slots(ersl_t *euler)
{
	uint8_t idx = 0;
	uint8_t count = 0;

	while (idx < MAX_AST_BRANCH) {
		if (euler->ast_rsv[idx].type == 0) {
			count++;
		}
		idx++;
	}

	return count;
}
ast_t *ast_add_node(ersl_t *euler, uint8_t type, ast_t *ast_right,
		    ast_t *ast_left)
{
	ast_t *ast_node = ast_malloc(euler);

	if (ast_node == NULL)
		return NULL;

	ast_node->type = type;

	//ast_write_value(euler, ast_node);

	ast_node->left = ast_left;
	ast_node->right = ast_right;

	return ast_node;
}

uint8_t ast_comp_val(ast_t *n1, ast_t *n2)
{
	if (n1->type != n2->type)
		return 0;

	if (ISNUMERIC(n1)) {
		if (n1->value.number == n2->value.number)
			return 1;
		return 0;
	} else if (ISLITERAL(n1)) {
		if (n1->value.literal == n2->value.literal)
			return 1;
		return 0;
	} else if (ISOPERATOR(n1) && ISOPERATOR(n2))
		return 1;
	return 0;
}
void ast_node_copy(ast_t *dest, ast_t *src)
{
	if (!dest || !src)
		return;
	dest->type = src->type;
	if (ISNUMERIC(src))
		dest->value.number = src->value.number;
	else if (ISLITERAL(src))
		dest->value.literal = src->value.literal;
	dest->left = src->left;
	dest->right = src->right;
}

#ifdef UNIX
#include <stdlib.h>

void print_tree_recursive(FILE *fp, ast_t *curr)
{
	fprintf(fp, "    \"%p\" [ label=\"", curr);

	switch (curr->type) {
	case INT:
	case FLOAT:
	case CONST:
		fprintf(fp, "%f", curr->value.number);
		break;
	case LETTER:
		fprintf(fp, "%c", curr->value.literal);
		break;
	case PLUS:
		fprintf(fp, "( + )");
		break;
	case MINUS:
		fprintf(fp, "( - )");
		break;
	case MULT:
		fprintf(fp, "( * )");
		break;
	case DIV:
		fprintf(fp, "( / )");
		break;
	case FACT:
		fprintf(fp, "( ! )");
		break;
	case EXP:
		fprintf(fp, "( ^ )");
		break;
	default:
		fprintf(fp, "(%03d)", curr->type);
		break;
	}
	fprintf(fp, "\" ];\n");
	if (curr->left) {
		print_tree_recursive(fp, curr->left);
		fprintf(fp, "    \"%p\" -> \"%p\" ;\n", curr, curr->left);
	}
	if (curr->right) {
		print_tree_recursive(fp, curr->right);
		fprintf(fp, "    \"%p\" -> \"%p\" ;\n", curr, curr->right);
	}
}

void ast_print(ast_t *tree)
{
	FILE *fp;
	fp = fopen("digraph.dot", "w"); //opening file
	fprintf(fp, "digraph {\n");
	if (tree)
		print_tree_recursive(fp, tree);
	fprintf(fp, "}\n");

	fflush(fp);
	system("graph-easy --input=digraph.dot --as=boxart");
	free(fp);
}
#endif
