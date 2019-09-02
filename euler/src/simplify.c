#include "../inc/euler.h"
#include "../parser/grammar.h"
#include "../parser/ast.h"

void simplify_addition_with_zero(ast_t *node, ersl_t *euler)
{
	if (node == NULL || !AST_ISNODE(node)) {
		return;
	}

	ast_t *tmp = NULL;

	/* Simplify addition with 0. 1+0+1 -> 1+1, 0+1 -> 1 */
	if ((tmp = AST_OTHER_CHILD(
		     node, AST_VALUE_CHECK_CHILDS_2(node, INT, FLOAT, number,
						    0))) != NULL) {
		switch (node->type) {
		case PLUS:
			ast_relink_node(euler, node, tmp);
			break;
		case MULT:
			ast_destroy_node(euler, node);
			return;
		default:
			break;
		}

		//(node->type == MULT) ? ast_destroy_node(euler, node);
	}

	simplify_addition_with_zero(node->left, euler);
	simplify_addition_with_zero(node->right, euler);
}

double simplify_combine_variable(ast_t *node, ersl_t *euler, char var,
				 double coefficient)
{
	ast_t *tmp = NULL;

	if (node == NULL || !AST_ISNODE(node) ||
	    !AST_TYPE_CHECK_2(node, PLUS, MULT)) {
		return coefficient;
	}

	if (AST_VALUE_CHECK_CHILDS(node, LETTER, literal, var)) {
		if (AST_TYPE_CHECK(node, PLUS)) {
			coefficient++;
			if (AST_TYPE_CHECK(node->left, LETTER)) {
				ast_relink_node(euler, node, node->right);
			} else {
				ast_relink_node(euler, node, node->left);
			}

		} else if (AST_TYPE_CHECK(node, MULT)) {
			tmp = AST_WHICH_CHILD_HAS_TYPE_3(node, INT, FLOAT,
							 CONST);
			if (tmp != NULL) {
				coefficient += tmp->value.number;
				tmp->value.number = 90;
			}
		}
	}

	coefficient =
		simplify_combine_variable(node->left, euler, var, coefficient);
	coefficient =
		simplify_combine_variable(node->right, euler, var, coefficient);

	return coefficient;
}

char simplify_variable_addition(ast_t *node, ersl_t *euler, char variable)
{
	if (node != NULL && !AST_ISNODE(node)) {
		return variable;
	}

	return 0;
}

void simplify_sum(ast_t *node, ersl_t *euler)
{
	simplify_addition_with_zero(node, euler);

	//simplify_variable_addition(node, euler, 0);
}

void simplify(ast_t *node, ersl_t *euler)
{
	ast_print(node);
	if (node == NULL || AST_ISLEAF(node)) {
		return;
	}

	switch (node->type) {
	case PLUS:
		simplify_sum(node, euler);
		break;
	default:
		break;
	}
}
