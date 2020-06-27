#include "../inc/euler.h"
#include "../parser/ast.h"
#include "../parser/grammar.h"
#include "../parser/symbol_table.h"

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

ast_t *simplify_rational_number(ast_t *u)
{
	int n, d, g;
	ast_t *tmp = ast_malloc();
	if (!tmp)
		return NULL;
	if (KIND(u, INT))
		return u;
	else if (KIND(u, DIV)) {
		ast_operand(u, 1, tmp);
		n = (int)tmp->value.number;
		ast_operand(u, 2, tmp);
		d = (int)tmp->value.number;
		if (n % d == 0)
			return ast_add_leaf_const(INT, n / d);
		else {
			g = integer_gcd(n, d);
			if (d > 0)
				return ast_add_node(
					DIV, ast_add_leaf_const(INT, n / g),
					ast_add_leaf_const(INT, d / g));
			else if (d < 0)
				return ast_add_node(
					DIV, ast_add_leaf_const(INT, -n / g),
					ast_add_leaf_const(INT, -d / g));
		}
	}
	return NULL;
}
/*

ast_t *simplify_rne_rec(ast_t *u)
{
	ast_t *v = ast_malloc();
	if (!v)
		return NULL;

	if (KIND(u, INT))
		return u;
	else if (KIND(u, DIV)) {
		if (denominator_fun(u) == 0)
			return NULL;
		else
			return u;
	} else if (num_of_operands(u) == 1) {
		v = simplify_rne_rec(operand(u, 1));
		if (!v)
			return NULL;
		else if (KIND(u, PLUS))
			return v;
		else if (KIND(u, MINUS))
			return eval_product(-1, v);
	}
}
*/
/*
ast_t *simplify_rne(ast_t *u)
{
	ast_t *v = simplify_rne_rec(u);

	if (!v)
		return NULL;
	return simplify_rational_number(v);
}
*/
