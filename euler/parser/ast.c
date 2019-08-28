#include "../inc/euler.h"
#include "ast.h"
#include "grammar.h"
#include "symbol_table.h"

void ast_clear(ersl_t *euler)
{
	euler->ast[0].type = 0;
}

static ast_t *ast_malloc(ersl_t *euler)
{
	uint8_t idx = 0;

	/* Check if the type is 0 which means its available cause 0 is reserved */
	while (euler->ast[idx].type != 0) {
		idx++;
		if (idx >= MAX_AST_BRANCH)
			return NULL; /* Array is full */
	}

	return (euler->ast + idx);
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

ast_t *ast_add_leaf(ersl_t *euler, uint8_t type)
{
	ast_t *ast = ast_malloc(euler);

	ast->type = type;

	ast_write_value(euler, ast);

	ast->left = NULL;
	ast->right = NULL;

	return ast;
}

ast_t *ast_add_leaf_const(ersl_t *euler, uint8_t type, double value)
{
	ast_t *ast = ast_malloc(euler);

	ast->type = type;

	ast->value.number = value;

	ast->left = NULL;
	ast->right = NULL;

	return ast;
}

ast_t *ast_add_node(ersl_t *euler, uint8_t type, ast_t *ast_left,
		    ast_t *ast_right)
{
	ast_t *ast_node = ast_malloc(euler);
	ast_node->type = type;

	ast_write_value(euler, ast_node);

	ast_node->left = ast_left;
	ast_node->right = ast_right;

	return ast_node;
}

#ifdef UNIX

int _print_t(ast_t *tree, int is_left, int offset, int depth, char s[20][255])
{
	char b[20];
	int width = 5;

	if (!tree)
		return 0;
	if (tree->type == INT || tree->type == FLOAT || tree->type == CONST) {
		sprintf(b, "%f", tree->value.number);
	} else if (tree->type == LETTER) {
		sprintf(b, "%c", tree->value.literal);
	} else {
		sprintf(b, "(%03d)", tree->type);
	}

	int left = _print_t(tree->left, 1, offset, depth + 1, s);
	int right =
		_print_t(tree->right, 0, offset + left + width, depth + 1, s);

#ifdef COMPACT
	for (int i = 0; i < width; i++)
		s[depth][offset + left + i] = b[i];

	if (depth && is_left) {
		for (int i = 0; i < width + right; i++)
			s[depth - 1][offset + left + width / 2 + i] = '-';

		s[depth - 1][offset + left + width / 2] = '.';

	} else if (depth && !is_left) {
		for (int i = 0; i < left + width; i++)
			s[depth - 1][offset - width / 2 + i] = '-';

		s[depth - 1][offset + left + width / 2] = '.';
	}
#else
	for (int i = 0; i < width; i++)
		s[2 * depth][offset + left + i] = b[i];

	if (depth && is_left) {
		for (int i = 0; i < width + right; i++)
			s[2 * depth - 1][offset + left + width / 2 + i] = '-';

		s[2 * depth - 1][offset + left + width / 2] = '+';
		s[2 * depth - 1][offset + left + width + right + width / 2] =
			'+';

	} else if (depth && !is_left) {
		for (int i = 0; i < left + width; i++)
			s[2 * depth - 1][offset - width / 2 + i] = '-';

		s[2 * depth - 1][offset + left + width / 2] = '+';
		s[2 * depth - 1][offset - width / 2 - 1] = '+';
	}
#endif

	return left + width + right;
}

void ast_print(ast_t *tree)
{
	char s[20][255];
	for (int i = 0; i < 20; i++)
		sprintf(s[i], "%80s", " ");

	_print_t(tree, 0, 0, 0, s);

	for (int i = 0; i < 20; i++)
		printf("%s\n", s[i]);
}
#endif
