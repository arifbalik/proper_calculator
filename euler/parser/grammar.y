%include {
#include <assert.h>
#include "tokenizer.h"
}

%token_type { double }
%extra_argument { ersl_t *euler }
%start_symbol   query
%stack_overflow {printf("stack_overflow\n");}
%parse_failure {printf("parser failed\n");}

/* Priorities */
%nonassoc EQ.
%left PLUS MINUS.
%left MULT DIV MOD FACT.
%left EXP.
%left LAPREN RPAREN.

%wildcard ANY.

/* Elementary Arithmetic */
query ::= elar(B). {
        euler->resultn.fraction = B;
        euler->type = FRACTION;
}

/* Elementary Arithmetic */
elar(A) ::= LETTER. { A = 1; }
elar(A) ::= INT(B).                       { A = B;       }
elar(A) ::= FLOAT(B).                     { A = B;       }
elar(A) ::= CONST(B).                     { A = B;       }
elar(A) ::= MINUS elar(B).                { A = -B;      }
elar(A) ::= PLUS elar(B).                 { A = B;       }
elar(A) ::= LPAREN elar(B) RPAREN.        { A = B;       }
elar(A) ::= elar(B)    PLUS      elar(C). { A = B + C;   }
elar(A) ::= elar(B)    MINUS     elar(C). { A = B - C;   }
elar(A) ::= elar(B)    MULT      elar(C). { A = B * C;   }
elar(A) ::= elar(B)    DIV       elar(C). { A = (C != 0) ? (B / C) : (euler->status = MTHE); }
elar(A) ::= elar(B)    EXP       elar(C). { A=B; for(uint8_t i = 1; i < C; i++) A *= B; }
elar(A) ::= elar(B)    MOD       elar(C). { A = fmod(B, C); }
elar(A) ::= elar(B)    FACT.              { A = 1; for(uint8_t i = B; i > 0; i--) A *= i; }
