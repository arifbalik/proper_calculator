#include "../inc/euler.h"
#include "grammar.h"

ast_t *ast_add_leaf(ast_t *ast, uint8_t type, void *value)
{
	ast->type = type;

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

	ast->left = NULL;
	ast->right = NULL;

	return ast;
}
