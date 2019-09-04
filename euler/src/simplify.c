#include "../inc/euler.h"
#include "../parser/grammar.h"
#include "../parser/ast.h"

void simplify_zero(ast_t *node, ersl_t *euler)
{
	if (node == NULL || !AST_ISNODE(node)) {
		return;
	}

	ast_t *tmp = NULL;

	if ((tmp = AST_VALUE_CHECK_CHILDS_2(node, INT, FLOAT, number, 0)) !=
	    NULL) {
		switch (node->type) {
		/* Simplify addition with 0.
		 * _number + 0 + _number -> _number + _number,
		 * 0+ _number -> _number
		 */
		case PLUS:
			ast_relink_node(euler, node,
					AST_OTHER_CHILD(node, tmp));
			break;
		/* Simplify subst. with 0. 0 - _number -> -_number,
		 * _number-0 -> _number
		 */
		case MINUS:
			(tmp == node->left) ?
				node->left = NULL :
				ast_relink_node(euler, node,
						AST_OTHER_CHILD(node, tmp));
			break;
		/* Simplify multiplication with 0. _number * 0 + _number -> 1,
		 * 0 * _number -> 0 * _number
		 * If the expression contains only multiplication with zero,
		 * we don't change it.
		 */
		case MULT:
			ast_destroy_node(euler, node);
			return;
		/* 0! -> 1 */
		case FACT:
			ast_relink_node(euler, node,
					ast_add_leaf_const(euler, INT, 1));
			break;
		/* 0 / _number -> 0,
		 *  _number / 0 ->  _number / 0,
		 *  0 / 0 ->  0 / 0
		 */
		case DIV:
		/* 0^_number -> 0, _number^0 -> 1, 0^0 -> 0^0 */
		case EXP:
			/* If they are both 0, do nothing */
			if (!((node->left && node->right) &&
			      AST_TYPE_CHECK(node->left, node->right->type) &&
			      AST_LEAF_NUMBER_VAL(node->left,
						  node->right->value.number)) &&
			    tmp == node->left) {
				ast_destroy_node(euler, node);
				return;
			} else if (AST_TYPE_CHECK(node, EXP)) {
				ast_relink_node(euler, node,
						ast_add_leaf_const(euler, INT,
								   1));
			}
			break;
		default:
			break;
		}
	}

	simplify_zero(node->left, euler);
	simplify_zero(node->right, euler);
}

void simplify_sum_variable(ast_t *node, ersl_t *euler, char variable,
			   double coefficient, ast_t *relink_node)
{
	if (node == NULL || !AST_ISNODE(node)) {
		return;
	}
	ast_t *tmp = NULL;

	if ((tmp = AST_TYPE_CHECK_CHILDS(node, LETTER)) != NULL &&
	    variable == 0) {
		variable = tmp->value.literal;
		relink_node = tmp;
	}

	/* Find if node has a variable */
	if ((tmp = AST_VALUE_CHECK_CHILDS(node, LETTER, literal, variable)) !=
	    NULL) {
		switch (node->type) {
		case PLUS:
			coefficient += 1;
			if (relink_node != tmp)
				ast_relink_node(euler, node,
						AST_OTHER_CHILD(node, tmp));

			break;
		case MINUS:
			coefficient -= 1;
			if (relink_node != tmp)
				ast_relink_node(euler, node,
						AST_OTHER_CHILD(node, tmp));

			break;
		case MULT:
			if (AST_TYPE_CHECK_3(AST_OTHER_CHILD(node, tmp), INT,
					     FLOAT, CONST)) {
				coefficient +=
					AST_OTHER_CHILD(node, tmp)->value.number;
				if (relink_node != tmp)
					ast_relink_node(euler, node,
							ast_add_leaf_const(
								euler, INT, 0));
			}
			break;
		}
	}

	simplify_sum_variable(node->left, euler, variable, coefficient, NULL);
	simplify_sum_variable(node->right, euler, variable, coefficient, NULL);

	if (relink_node) {
		ast_relink_node(
			euler, relink_node,
			ast_add_node(euler, MULT,
				     ast_add_leaf_const(euler, FLOAT,
							coefficient),
				     ast_add_leaf_literal(euler, LETTER,
							  tmp->value.literal)));
		//relink_node = vnode;
	}
}

void simplify_sum(ast_t *node, ersl_t *euler)
{
	//ast_t *vnode;

	simplify_sum_variable(node, euler, 0, 0, NULL);
	simplify_zero(node, euler);
}

void simplify(ast_t *node, ersl_t *euler)
{
	ast_print(node);
	if (node == NULL || AST_ISLEAF(node)) {
		return;
	}

	simplify_sum(node, euler);
}
