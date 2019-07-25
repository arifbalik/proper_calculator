%include {
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "includes.h"
#include "variables.h"
#include "euler/inc/euler.h"

int __err;
}

%token_type { struct token_info }

%code {

}

%nonassoc EQ.
%left PLUS MINUS.
%left MULT DIV.
%left LAPREN RPAREN.

%parse_accept { }

%syntax_error { printf("Syntax Error!\n"); }

%parse_failure { }

%start_symbol program

program ::= line.

line ::= expression(C). {
        if(__err >= 0){
                printf("%f\n", C.val);
                C.val = 0;

        }else{
                __err = 0;
        }
}



expression ::= LIST. { list_vars(); }

f_form(A) ::= LPAREN expression(B) RPAREN. { A.val = B.val; }

math_function(A) ::= SIN f_form(B). { A.val = E_SIN(B.val); }
math_function(A) ::= ASIN f_form(B). { A.val = E_ASIN(B.val); }
math_function(A) ::= SINH f_form(B). { A.val = E_SINH(B.val); }

math_function(A) ::= COS f_form(B). { A.val = E_COS(B.val); }
math_function(A) ::= ACOS f_form(B). { A.val = E_ACOS(B.val); }
math_function(A) ::= COSH f_form(B). { A.val = E_COSH(B.val); }

math_function(A) ::= TAN f_form(B). { A.val = E_TAN(B.val); }
math_function(A) ::= ATAN f_form(B). { A.val = E_ATAN(B.val); }
math_function(A) ::= TANH f_form(B). { A.val = E_TANH(B.val); }

math_function(A) ::= COT f_form(B). { A.val = E_COT(B.val); }


math_function(A) ::= EXP f_form(B). { A.val = E_EXP(B.val); }
math_function(A) ::= LOG f_form(B). { A.val = E_LOG(B.val); }
math_function(A) ::= LN f_form(B). { A.val = E_LN(B.val); }

math_function(A) ::= POW LPAREN expression(B) COMMA expression(C) RPAREN. { A.val = E_POW(B.val, C.val); }
math_function(A) ::= SQRT f_form(B). { A.val = E_SQRT(B.val); }
math_function(A) ::= CBRT f_form(B). { A.val = E_CBRT(B.val); }

math_function(A) ::= CEIL f_form(B). { A.val = E_CEIL(B.val); }
math_function(A) ::= FLOOR f_form(B). { A.val = E_FLOOR(B.val); }
math_function(A) ::= MOD LPAREN expression(B) COMMA expression(C) RPAREN. { A.val = E_MOD(B.val, C.val); }

math_function(A) ::= ABS f_form(B). { A.val = E_ABS(B.val); }


expression(A) ::= math_function(B). { A.val = B.val; }

expression(A) ::= VAR(B). { A.val = get_var_val(B.name, -1); }
expression(A) ::= VAR EQ expression(B). { A.val = add_var(fvar, B.val); }


expression(A) ::= expression(B) PLUS expression(C). { A.val = B.val + C.val; }
expression(A) ::= expression(B) MINUS expression(C). { A.val = B.val - C.val;  }
expression(A) ::= expression(B) MULT expression(C). { A.val = B.val * C.val; }
expression(A) ::= expression(B) DIV expression(C). {
        if(C.val != 0)
                A.val = B.val / C.val;
        else
                printf("Math Error!");
}
expression(A) ::= MINUS expression(B). { A.val = -B.val; }
expression(A) ::= PLUS expression(B). { A.val = B.val; }

expression(A) ::= LPAREN expression(B) RPAREN. { A.val = B.val; }

expression ::= constant.

constant(A) ::= PI. {A.val = M_PI;}

expression(A) ::= number(B). {A.val = B.val;}
number ::= INT.
number ::= FLOAT.













expression ::= DUMMY.
