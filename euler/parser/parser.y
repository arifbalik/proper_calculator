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

%token_type     { token_val     }
%extra_argument { ersl_t *euler }
%start_symbol   query
%stack_overflow {printf("stack_overflow\n");}
%parse_failure {printf("parser failed\n");}

/* Priorities */
%nonassoc EQ.
%left PLUS MINUS.
%left MULT DIV.
%left LAPREN RPAREN.

%nonassoc TO FROM EVAL FN_CALL LIST QUIT STEPS INTEGRATE.

%wildcard ANY.

/* Basic Math Expressions */
query ::= mathb(B). {
        euler->type = FRACTION;
        euler->resultn.fraction = B.val;
}
/* Variable assignment */
query ::= varas.
/* Function assignment */
query ::= fnas.
query ::= fncall.


fncall ::= LETTER LPAREN mathb RPAREN. {
        euler->status = _FEX;
}



/* User defined custom functions */
fnas ::= LETTER(B) LPAREN LETTER(C) RPAREN EQ ANY. {
        add_func(B.name[0], C.name[0], get_raw_after_equal());
        free_raw_after_equal();
        euler->type = NO_RESULT;
}

/* Variable assign */
varas ::=     LETTER(B).              { euler->status = get_var_val(B.name, -1, &euler->resultn.fraction); euler->type = FRACTION; }
varas(A) ::=  LETTER(B) EQ mathb(C).  { euler->status = add_var(B.name, C.val ); euler->status = get_var_val(B.name, -1, &A.val); euler->type = FRACTION; }

/* Numbers */
num(A) ::= INT(B).        { A.val = B.val; }
num(A) ::= FLOAT(B).      { A.val = B.val; }

/* Basic Algebra Arithmetic */
mathb(A) ::= num(B).                         { A.val = B.val;           }
mathb(A) ::= f_form(B).                      { A.val = B.val;           }
mathb(A) ::= std_function(B).                { A.val = B.val;           }
mathb(A) ::= CONST(B).                       { A.val = B.val;           }
mathb(A) ::= MINUS mathb(B).                 { A.val = -B.val;          }
mathb(A) ::= PLUS mathb(B).                  { A.val = B.val;           }
mathb(A) ::= mathb(B)    PLUS      mathb(C). { A.val = B.val + C.val;   }
mathb(A) ::= mathb(B)    MINUS     mathb(C). { A.val = B.val - C.val;   }
mathb(A) ::= mathb(B)    MULT      mathb(C). { A.val = B.val * C.val;   }
mathb(A) ::= mathb(B)    DIV       mathb(C). { A.val = (C.val != 0) ? (B.val / C.val) : (euler->status = MTHE); }

/* Standart Math Functions. */
f_form(A) ::= LPAREN mathb(B) RPAREN.           { A.val = B.val;                }
std_function(A) ::= SIN         f_form(B).      { A.val = E_SIN(B.val);         }
std_function(A) ::= ASIN        f_form(B).      { A.val = E_ASIN(B.val);        }
std_function(A) ::= SINH        f_form(B).      { A.val = E_SINH(B.val);        }
std_function(A) ::= COS         f_form(B).      { A.val = E_COS(B.val);         }
std_function(A) ::= ACOS        f_form(B).      { A.val = E_ACOS(B.val);        }
std_function(A) ::= COSH        f_form(B).      { A.val = E_COSH(B.val);        }
std_function(A) ::= TAN         f_form(B).      { A.val = E_TAN(B.val);         }
std_function(A) ::= ATAN        f_form(B).      { A.val = E_ATAN(B.val);        }
std_function(A) ::= TANH        f_form(B).      { A.val = E_TANH(B.val);        }
std_function(A) ::= COT         f_form(B).      { A.val = E_COT(B.val);         }
std_function(A) ::= EXP         f_form(B).      { A.val = E_EXP(B.val);         }
std_function(A) ::= LOG         f_form(B).      { A.val = E_LOG(B.val);         }
std_function(A) ::= LN          f_form(B).      { A.val = E_LN(B.val) ;         }
std_function(A) ::= SQRT        f_form(B).      { A.val = E_SQRT(B.val);        }
std_function(A) ::= CBRT        f_form(B).      { A.val = E_CBRT(B.val);        }
std_function(A) ::= CEIL        f_form(B).      { A.val = E_CEIL(B.val);        }
std_function(A) ::= FLOOR       f_form(B).      { A.val = E_FLOOR(B.val);       }
std_function(A) ::= ABS         f_form(B).      { A.val = E_ABS(B.val);         }
std_function(A) ::= POW LPAREN mathb(B) COMMA mathb(C) RPAREN. { A.val = E_POW(B.val, C.val); }
std_function(A) ::= MOD LPAREN mathb(B) COMMA mathb(C) RPAREN. { A.val = E_MOD(B.val, C.val); }



%code {
#pragma GCC diagnostic pop
}
