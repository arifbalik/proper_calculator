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
%nonassoc EQ INT FLOAT UNKNOWN TO SIGMA PROD.
%left CONST.
%left LETTER.
%left PLUS MINUS.
%left MULT DIV MOD FACT.
%left EXP.
%left AND OR NOT GREATER SMALLER XOR BAND BOR ISEQ.
%left LPAREN RPAREN.

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
elar(A) ::= SIGMA LPAREN ANY COMMA LETTER COMMA number(B) COMMA number(C) RPAREN. {

        char fn[MAX_QUERY_LENGTH];
        char query[MAX_QUERY_LENGTH];
	ersl_t tmp;
	double result = 0;

        st_get_string_between_tokens(&(euler->symbol_table), LPAREN, COMMA, fn, 1);

        while(B <= C){
                strplace(fn, query, st_get_letter(&(euler->symbol_table)), (int)B);
                tmp.ascii = query;
                parse_query(&tmp);
                result += tmp.resultn.fraction;
                B++;
        }

        A = result;


}

elar(A) ::= PROD LPAREN ANY COMMA LETTER COMMA number(B) COMMA number(C) RPAREN. {

        char fn[MAX_QUERY_LENGTH];
        char query[MAX_QUERY_LENGTH];
	ersl_t tmp;
	double result = 0;

        st_get_string_between_tokens(&(euler->symbol_table), LPAREN, COMMA, fn, 1);

        while(B <= C){
                strplace(fn, query, st_get_letter(&(euler->symbol_table)), (int)B);
                tmp.ascii = query;
                parse_query(&tmp);
                result *= tmp.resultn.fraction;
                B++;
        }

        A = result;


}


/* Boolean Operators */
elar(A) ::= elar(B) AND elar(C). { A = (int)B & (int)C; }
elar(A) ::= elar(B) NOT AND elar(C). { A = ~((int)B & (int)C);  }
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
elar(A) ::= elar(B)    PLUS      elar(C). { euler->resultn.fraction = A = B + C;   }
elar(A) ::= elar(B)    MINUS     elar(C). { A = B - C;   }
elar(A) ::= elar(B)    MULT      elar(C). { A = B * C;   }
elar(A) ::= elar(B)    DIV       elar(C). { A = (C != 0) ? (B / C) : (0); euler->status = MTHE; }
elar(A) ::= elar(B)    EXP       elar(C). {A = pow(B,C); }
elar(A) ::= elar(B)    MOD       elar(C). { A = fmod(B, C); }
elar(A) ::= elar(B)    FACT.              { A = 1; for(uint8_t i = B; i > 0; i--) A *= i; }

number(A) ::= FLOAT(B).                     { A = B;       }
number(A) ::= INT(B).                       { A = B;       }
number(A) ::= constant(B).                  { A = B;       }

constant(A) ::= PI. { A = _PI; }
constant(A) ::= E. { A = _E; }
