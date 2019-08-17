%include {
#include <assert.h>
#include "tokenizer.h"
#include "grammar.h"
#include "../inc/constant.h"
}

%token_type { double }
%extra_argument { ersl_t *euler }
%start_symbol   query
%stack_overflow {printf("stack_overflow\n");}
%parse_failure {printf("parser failed\n");}
%parse_failure {printf("syntax error\n");}

/* Priorities */
%nonassoc EQ INT FLOAT UNKNOWN TO SIGMA.
%left CONST.
%left LETTER.
%left PLUS MINUS.
%left MULT DIV MOD FACT.
%left EXP.
%left AND OR NOT GREATER SMALLER XOR BAND BOR ISEQ.
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


/* Iterated Functions. */
query ::= SIGMA ANY COMMA LETTER EQ number(B) TO number(C). {
        char fn[MAX_QUERY_LENGTH];
        char str[MAX_QUERY_LENGTH];
        char letter = get_letter();
        double res = 0;

        if(count_token(COMMA) > 1){
                /* error */
                euler->status = MTHE;
                goto end;
        }
        get_string_between_tokens(SIGMA, COMMA, fn);
        for(uint8_t i = B; i<=C; i++){
                strplace(fn, str, letter, i);
                _strcpy(euler->ascii, str, MAX_QUERY_LENGTH);
                parse_query(euler);
                res += euler->resultn.fraction;
        }
        euler->resultn.fraction = res;
        euler->status = FRACTION;
        end:
        euler->status = euler->status;

}


/* Boolean Operators */
elar(A) ::= elar(B) AND elar(C). { A = (int)B & (int)C; }
elar(A) ::= elar(B) NOT AND elar(C). { A = ~((int)B & (int)C); }
elar(A) ::= elar(B) OR elar(C). { A = (int)B | (int)C; }
elar(A) ::= elar(B) NOT OR elar(C). { A = ~((int)B | (int)C); }
elar(A) ::= elar(B) XOR elar(C). { A = ((int)B ^ (int)C); }
elar(A) ::= elar(B) GREATER GREATER elar(C). { A = ((int)B >> (int)C); }
elar(A) ::= elar(B) SMALLER SMALLER elar(C). { A = ((int)B << (int)C); }
elar(A) ::= NOT elar(B). { A =  ~(int)B; }

/* Binary Relations */
elar(A) ::= elar(B) BAND elar(C). { A = ((int)B && (int)C); }
elar(A) ::= elar(B) BOR elar(C). { A = ((int)B || (int)C); }
elar(A) ::= elar(B) ISEQ elar(C). { A = ((int)B == (int)C); }
elar(A) ::= elar(B) GREATER elar(C). { A = ((int)B > (int)C); }
elar(A) ::= elar(B) SMALLER elar(C). { A = ((int)B < (int)C); }

/* Elementary Arithmetic */
elar(A) ::= LETTER. { A = 1; }
elar(A) ::= number(B).                    { A = B;       }
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
number(A) ::= constant(B).                  { A = B;       }

constant(A) ::= PI. { A = _PI; }
constant(A) ::= E. { A = _E; }
