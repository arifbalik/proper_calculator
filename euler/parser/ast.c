#include "../inc/euler.h"
#include "ast.h"
#include "grammar.h"
#include "symbol_table.h"

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

void ast_finalize(ersl_t *euler)
{
	euler->ast_top_idx = ast_get_root_idx(euler);
}

/* return the address of the next available element */
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
ast_t *ast_add_node(ersl_t *euler, uint8_t type, ast_t *ast_left,
		    ast_t *ast_right)
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
