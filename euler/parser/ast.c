#include "../inc/euler.h"
#include "grammar.h"

static void ast_write_value(ast_t *ast)
{
	switch (ast->type) {
	case INT:
	case FLOAT:
		ast->value.number = *((double *)value);
		break;
	case LETTER:
		ast->value.literal = (char)(*((char *)value));
		break;
	default:
		return NULL; /* Unsupported type */
		break;
	}
}

ast_t *ast_add_leaf(ast_t *ast, uint8_t type, void *value)
{
	ast->type = type;

	ast_write_value(ast);

	ast->left = NULL;
	ast->right = NULL;

	return ast;
}

ast_t *add_node(ast_t *ast_node, uint8_t type, void *value, ast_t *ast_left,
		ast_t *ast_right)
{
	ast_node->type = type;

	ast_write_value(ast_node);

	ast_node->left = ast_left;
	ast_node->right = ast_right;

	return ast_node;
}
