%include {
#include <assert.h>
#include "lexer.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wredundant-decls"
#pragma GCC diagnostic ignored "-Wstrict-prototypes"
#pragma GCC diagnostic ignored "-Wmissing-prototypes"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wsign-compare"

}

%token_type { token_val }
%extra_argument { ersl_t *euler }
%parse_accept { }

%syntax_error { }
%parse_failure { }
%start_symbol query

/* Priorities */
%nonassoc EQ.
%left PLUS MINUS.
%left MULT DIV.
%left LAPREN RPAREN.

query ::= re_arithmetic(B). {
        euler->type = FRACTION;
        euler->result.fraction = B.val;
}

query ::= calculus(B). {
        euler->type = FRACTION;
        euler->result.fraction = B.val;
}

query ::= cmd.

cmd ::= QUIT.
cmd ::= DUMMY.
cmd ::= LIST. { list_vars(); }

/* Calculus */
calculus(A) ::= INTEGRATE SIN FROM VAR EQ re_number(C) TO re_number(D). {
        for(double a = C.val; a <= D.val; a += (D.val - C.val) / 10000){
                A.val += ((D.val - C.val) / 10000) * E_SIN(a);
        }
}

/* Standart Math Functions. */
f_form(A) ::= LPAREN re_arithmetic(B) RPAREN. { A.val = B.val; }

std_function(A) ::= SIN f_form(B). { A.val = E_SIN(B.val); }
std_function(A) ::= ASIN f_form(B). { A.val = E_ASIN(B.val); }
std_function(A) ::= SINH f_form(B). { A.val = E_SINH(B.val); }
std_function(A) ::= COS f_form(B). { A.val = E_COS(B.val); }
std_function(A) ::= ACOS f_form(B). { A.val = E_ACOS(B.val); }
std_function(A) ::= COSH f_form(B). { A.val = E_COSH(B.val); }
std_function(A) ::= TAN f_form(B). { A.val = E_TAN(B.val); }
std_function(A) ::= ATAN f_form(B). { A.val = E_ATAN(B.val); }
std_function(A) ::= TANH f_form(B). { A.val = E_TANH(B.val); }
std_function(A) ::= COT f_form(B). { A.val = E_COT(B.val); }
std_function(A) ::= EXP f_form(B). { A.val = E_EXP(B.val); }
std_function(A) ::= LOG f_form(B). { A.val = E_LOG(B.val); }
std_function(A) ::= LN f_form(B). { A.val = E_LN(B.val) ; }
std_function(A) ::= POW LPAREN re_arithmetic(B) COMMA re_arithmetic(C) RPAREN. { A.val = E_POW(B.val, C.val); }
std_function(A) ::= SQRT f_form(B). { A.val = E_SQRT(B.val); }
std_function(A) ::= CBRT f_form(B). { A.val = E_CBRT(B.val); }
std_function(A) ::= CEIL f_form(B). { A.val = E_CEIL(B.val); }
std_function(A) ::= FLOOR f_form(B). { A.val = E_FLOOR(B.val); }
std_function(A) ::= MOD LPAREN re_arithmetic(B) COMMA re_arithmetic(C) RPAREN. { A.val = E_MOD(B.val, C.val); }
std_function(A) ::= ABS f_form(B). { A.val = E_ABS(B.val); }

/* Basic Algebra Arithmetic */
re_arithmetic(A) ::= re_number(B). { A.val = B.val; }
re_arithmetic(A) ::= f_form(B). { A.val = B.val; }
re_arithmetic(A) ::= std_function(B). { A.val = B.val; }
re_arithmetic(A) ::= CONST(B). { A.val = B.val; }
re_arithmetic(A) ::= VAR(B). { euler->error = get_var_val(B.name, -1, &A.val); }
re_arithmetic(A) ::= VAR(B) EQ re_arithmetic(C). { add_var(B.name, C.val ); get_var_val(B.name, -1, &A.val); }
re_arithmetic(A) ::= MINUS re_arithmetic(B). { A.val = -B.val; }
re_arithmetic(A) ::= PLUS re_arithmetic(B). { A.val = B.val; }
re_arithmetic(A) ::= re_arithmetic(B) PLUS re_arithmetic(C). { A.val =   B.val + C.val; }
re_arithmetic(A) ::= re_arithmetic(B) MINUS re_arithmetic(C). { A.val = B.val - C.val; }
re_arithmetic(A) ::= re_arithmetic(B) MULT re_arithmetic(C). { A.val = B.val * C.val; }
re_arithmetic(A) ::= re_arithmetic(B) DIV re_arithmetic(C). { A.val = (C.val != 0)? (B.val / C.val) : (euler->error = MTHE); }

/* Numbers */
re_number(A) ::= INT(B). { A.val = B.val; }
re_number(A) ::= FLOAT(B). { A.val = B.val; }


%code {
#pragma GCC diagnostic pop
}
