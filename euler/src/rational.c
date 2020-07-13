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
	if (ISNUMERIC(u))
		return u;
	else if (KIND(u, DIV)) {
		tmp = ast_operand(u, 1);
		n = (int)tmp->value.number;
		tmp = ast_operand(u, 2);
		d = (int)tmp->value.number;
		if (n % d == 0)
			return LEAF_C(INT, n / d);
		else {
			g = integer_gcd(n, d);
			if (d > 0)
				return NODE(DIV, LEAF_C(INT, n / g),
					    LEAF_C(INT, d / g));
			else if (d < 0)
				return NODE(DIV, LEAF_C(INT, -n / g),
					    LEAF_C(INT, -d / g));
		}
	} else
		return u;

	return NULL;
}

ast_t *eval_sum(ast_t *u, ast_t *v)
{
	if (!u)
		return v;
	else if (!v)
		return u;
	else if ((ISNUMERIC(u) || KIND(u, DIV)) &&
		 (KIND(v, DIV) || ISNUMERIC(v)))
		return NODE(DIV,
			    LEAF_C(FLOAT,
				   NUMERATOR(u) * DENOMINATOR(v) +
					   NUMERATOR(v) * DENOMINATOR(u)),
			    LEAF_C(FLOAT, DENOMINATOR(u) * DENOMINATOR(v)));
	else
		return NODE(PLUS, u, v);
}

ast_t *eval_subs(ast_t *u, ast_t *v)
{
	if (!u)
		return v;
	else if (!v)
		return u;
	else if ((ISNUMERIC(u) || KIND(u, DIV)) &&
		 (KIND(v, DIV) || ISNUMERIC(v)))
		return NODE(DIV,
			    LEAF_C(FLOAT,
				   NUMERATOR(u) * DENOMINATOR(v) -
					   NUMERATOR(v) * DENOMINATOR(u)),
			    LEAF_C(FLOAT, DENOMINATOR(u) * DENOMINATOR(v)));
	else
		return NODE(MINUS, u, v);
}

ast_t *eval_prod(ast_t *u, ast_t *v)
{
	if (!u)
		return v;
	else if (!v)
		return u;
	else if ((ISNUMERIC(u) || KIND(u, DIV)) &&
		 (KIND(v, DIV) || ISNUMERIC(v)))
		return NODE(DIV, LEAF_C(FLOAT, NUMERATOR(u) * NUMERATOR(v)),
			    LEAF_C(FLOAT, DENOMINATOR(u) * DENOMINATOR(v)));
	else
		return NODE(MULT, u, v);
}
ast_t *eval_quot(ast_t *v, ast_t *w)
{
	if (NUMERATOR(v) == 0)
		return NULL;
	return NODE(DIV, LEAF_C(INT, NUMERATOR(v) * DENOMINATOR(w)),
		    LEAF_C(INT, NUMERATOR(w) * DENOMINATOR(v)));
}

ast_t *eval_pow(ast_t *v, int n)
{
	ast_t *s = ast_malloc();
	if (NUMERATOR(v) != 0) {
		if (n > 0) {
			s = eval_pow(v, n - 1);
			return eval_prod(s, v);
		} else if (n == 0)
			return LEAF_C(INT, 1);
		else if (n == -1)
			return NODE(DIV, LEAF_C(INT, 1), v);
		else if (n < -1) {
			s = eval_pow(v, n + 1);
			return eval_quot(s, v);
		}
	} else {
		if (n >= 1)
			return LEAF_C(INT, 1);
		else
			return NULL;
	}
	return NULL;
}

ast_t *simplify_rne_rec(ast_t *u)
{
	ast_t *v = ast_malloc();
	ast_t *w = ast_malloc();
	if (!v)
		return NULL;

	if (ISNUMERIC(u))
		return u;

	else if (KIND(u, MOD)) {
		if (DENOMINATOR(u) == 0)
			return NULL;
		else
			return u;
	} else if (NOPS(u) == 1) {
		v = simplify_rne_rec(ast_operand(u, 1));
		if (!v)
			return NULL;
		else if (KIND(u, PLUS))
			return v;
		else if (KIND(u, MINUS))
			return eval_prod(LEAF_C(INT, -1), v);
	} else if (NOPS(u) == 2) {
		if (KIND(u, PLUS) || KIND(u, MINUS) || KIND(u, MULT) ||
		    KIND(u, DIV)) {
			v = simplify_rne_rec(ast_operand(u, 1));
			w = simplify_rne_rec(ast_operand(u, 2));
			if (!v || !w)
				return NULL;
			else {
				if (KIND(u, PLUS))
					return eval_sum(v, w);
				else if (KIND(u, MINUS))
					return eval_subs(v, w);
				else if (KIND(u, MULT))
					return eval_prod(v, w);
				else if (KIND(u, DIV))
					return simplify_rational_number(u);
			}
		} else if (KIND(u, EXP)) {
			v = simplify_rne_rec(ast_operand(u, 1));
			if (!v)
				return NULL;
			else
				return eval_pow(v, NVAL(ast_operand(u, 2)));
		}
	}
	return NULL;
}

ast_t *simplify_rne(ast_t *u)
{
	ast_t *v = simplify_rne_rec(u);
	if (!v)
		return NULL;
	return simplify_rational_number(v);
}
