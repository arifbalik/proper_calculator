#include "../inc/euler.h"
#include "../parser/grammar.h"
#include "../parser/ast.h"

#define ZERO_CHECK(n) AST_VALUE_CHECK_CHILDS_2(n, INT, FLOAT, number, 0)
#define VAR_CHECK(n, v) AST_VALUE_CHECK_CHILDS(n, LETTER, literal, v)

void ast_print_element_list(ersl_t *euler)
{
	uint8_t idx = 0;
#ifdef UNIX
	while (idx <= euler->ast_element_idx) {
		switch (euler->ast_element_type[idx]) {
		case FLOAT:
		case CONST:
		case INT:
			printf("%f|", euler->ast_element[idx].number);

			break;
		case LETTER:
			printf("%c|", euler->ast_element[idx].literal);

			break;
		default:
			if (euler->ast_element_type[idx] != 0)
				printf("(%d)|",
				       euler->ast_element[idx].node->type);
			else
				printf("(unknown)");
			break;
		}

		idx++;
	}
#endif
}

void ast_sort_element_list(ersl_t *euler)
{
	uint8_t idx = 0, inner_idx = 0;

	while (idx <= euler->ast_element_idx) {
		if (euler->ast_element_type[idx] == 0) {
			inner_idx = idx;
			while (inner_idx < euler->ast_element_idx) {
				if (euler->ast_element_type[inner_idx] != 0) {
					euler->ast_element_type[idx] =
						euler->ast_element_type
							[inner_idx];
					euler->ast_element[idx] =
						euler->ast_element[inner_idx];
					euler->ast_element_type[inner_idx] = 0;
					break;
				}
				inner_idx++;
			}
		}
		idx++;
	}
	idx = 0;
	while (euler->ast_element_type[idx] != 0) {
		idx++;
	}
	euler->ast_element_idx = idx - 1;
}
void ast_new_element(ersl_t *euler, ast_t *node)
{
	if (node == NULL || !AST_ISNODE(node)) {
		return;
	}

	ast_t *child = node->left;

	while (child) {
		switch (child->type) {
		case PLUS: /* ignore */
			break;
		case INT:
		case FLOAT:
		case CONST:

			euler->ast_element[euler->ast_element_idx].number =
				child->value.number;
			euler->ast_element_type[euler->ast_element_idx] = FLOAT;
			euler->ast_element_idx++;
			break;
		case LETTER:

			euler->ast_element[euler->ast_element_idx].node =
				ast_add_node(euler, MULT,
					     ast_add_leaf_const(euler, FLOAT,
								1),
					     ast_add_leaf_literal(
						     euler, LETTER,
						     child->value.literal));
			euler->ast_element_type[euler->ast_element_idx] = MULT;
			euler->ast_element_idx++;
			break;
		default:
			euler->ast_element[euler->ast_element_idx].node = child;
			euler->ast_element_type[euler->ast_element_idx] =
				child->type;
			euler->ast_element_idx++;
			break;
		}

		child = (child == node->left) ? node->right : NULL;
	}
}

ast_t *ast_create_tree_from_element_list(ersl_t *euler)
{
	double number = 0;
	char variable = 0;
	uint8_t idx = 0, inner_idx = 0;
	ast_t *new_tree = NULL, *letter_node = NULL, *tmp = NULL;

	while (idx < euler->ast_element_idx) {
		switch (euler->ast_element_type[idx]) {
		case INT:
		case FLOAT:
		case CONST:
			number += euler->ast_element[idx].number;
			euler->ast_element_type[idx] = 0;
			break;
		case MULT:

			if ((letter_node = AST_TYPE_CHECK_CHILDS(
				     euler->ast_element[idx].node, LETTER)) !=
			    NULL) {
				inner_idx = idx + 1;
				variable = letter_node->value.literal;

				while (inner_idx < euler->ast_element_idx) {
					if (euler->ast_element_type[inner_idx] ==
						    MULT &&
					    (tmp = VAR_CHECK(
						     euler->ast_element[inner_idx]
							     .node,
						     variable)) != NULL) {
						tmp = AST_OTHER_CHILD(
							euler->ast_element
								[inner_idx]
									.node,
							tmp);

						euler->ast_element[idx]
							.node = ast_add_node(
							euler, MULT,
							ast_add_leaf_literal(
								euler, LETTER,
								variable),
							ast_add_node(
								euler, PLUS,
								tmp,
								AST_OTHER_CHILD(
									euler->ast_element
										[idx]
											.node,
									AST_TYPE_CHECK_CHILDS(
										euler->ast_element
											[idx]
												.node,
										LETTER))));

						euler->ast_element_type
							[inner_idx] = 0;
					}
					inner_idx++;
				}
			}

			break;
		}
		idx++;
	}
	if (number > 0) {
		euler->ast_element[euler->ast_element_idx].number = number;
		euler->ast_element_type[euler->ast_element_idx] = FLOAT;
		euler->ast_element_idx++;
	}

	ast_sort_element_list(euler);

	ast_print_element_list(euler);

	idx = 1;
	switch (euler->ast_element_type[0]) {
	case FLOAT:
		new_tree = ast_add_leaf_const(euler, FLOAT,
					      euler->ast_element[0].number);
		break;
	default:
		new_tree = euler->ast_element[0].node;
		break;
	}

	while (idx <= euler->ast_element_idx) {
		if (euler->ast_element_type[idx] == FLOAT)
			new_tree = ast_add_node(
				euler, PLUS, new_tree,
				ast_add_leaf_const(
					euler, FLOAT,
					euler->ast_element[idx].number));
		else
			new_tree = ast_add_node(euler, PLUS, new_tree,
						euler->ast_element[idx].node);
		idx++;
	}
	euler->ast_element_idx = 0;

	return new_tree;
}
ast_t *simplify_sum(ast_t *node, ersl_t *euler, uint8_t depth)
{
	if (node == NULL || !AST_ISNODE(node) || node->type != PLUS) {
		return NULL;
	}

	ast_new_element(euler, node);

	simplify_sum(node->left, euler, depth + 1);
	simplify_sum(node->right, euler, depth + 1);

	if (!depth) {
		return ast_create_tree_from_element_list(euler);
	}
	return NULL;
}

void simplify(ast_t *root, ast_t *child, ersl_t *euler)
{
	ast_t *changed_node = NULL;
	if (child == NULL) {
		return;
	}

	if (child->type == PLUS) {
		changed_node = simplify_sum(child, euler, 0);
		if (changed_node != NULL) {
			ast_relink_node(euler, child, changed_node);
			child = changed_node;
			ast_print(changed_node);
			changed_node = NULL;
			printf("\n\navailable slots for ast %d\n\n",
			       ast_get_available_slots(euler));
		}
	}

	simplify(root, child->left, euler);
	simplify(root, child->right, euler);
}
