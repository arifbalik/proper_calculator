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
%nonassoc EQ INT FLOAT UNKNOWN.
%left CONST.
%left LETTER.
%left AND OR NOT.
%left PLUS MINUS.
%left MULT DIV MOD FACT.
%left EXP.
%left LAPREN RPAREN.

%default_destructor {
        euler->type = euler->type;
}

%wildcard ANY.

/* Elementary Arithmetic */
query ::= elar(B) EOQ. {
        euler->resultn.fraction = B;
        euler->type = FRACTION;
}

query ::= boolean(B) EOQ. {
        euler->resultn.fraction = B;
        euler->type = BINARY;
}

/* Boolean Operators */
boolean(A) ::= elar(B) AND elar(C). { A = (int)B & (int)C; }
boolean(A) ::= elar(B) NOT AND elar(C). { A = ~((int)B & (int)C); }
boolean(A) ::= elar(B) OR elar(C). { A = (int)B | (int)C; }
boolean(A) ::= elar(B) NOT OR elar(C). { A = ~((int)B | (int)C); }
boolean(A) ::= NOT elar(B). { A =  ~(int)B; }



/* Elementary Arithmetic */
elar(A) ::= LETTER. { A = 1; }
elar(A) ::= number(B).                    { A = B;       }
elar(A) ::= LPAREN boolean(B) RPAREN.     { A = B;       }
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

number(A) ::= FLOAT(B).                     { A = B;       }
number(A) ::= INT(B).                       { A = B;       }
