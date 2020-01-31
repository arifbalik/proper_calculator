%include {
#include <assert.h>
#include "../inc/euler.h"
#include "../inc/constant.h"
#include "../inc/simplify.h"
#include "symbol_table.h"
#include "grammar.h"
#include "ast.h"
}

%token_type { ast_t * }
%extra_argument { ersl_t *euler }
%start_symbol   query
%stack_overflow {printf("stack_overflow\n");}
%parse_failure {printf("parser failed\n");}
%parse_failure {printf("syntax error\n");}

/* Priorities */
%nonassoc EQ INT FLOAT UNKNOWN TO SIGMA PROD PI E COMMA SHIFTL SHIFTR NAND NOR.
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

        ast_finalize(euler);
        simplify(B, B, euler);
        ast_print(ast_get_root(euler));
        ast_init(euler);
}


/* Iterated Functions. */
/* elar(A) ::= SIGMA LPAREN ANY COMMA LETTER COMMA number(B) COMMA number(C) RPAREN. {

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


} */


/* Boolean Operators */
elar(A) ::= elar(B) AND elar(C). {A = ast_add_node(euler, AND, B, C); }
elar(A) ::= elar(B) NOT AND elar(C). { A = ast_add_node(euler, NAND, B, C);  }
elar(A) ::= elar(B) OR elar(C). { A = ast_add_node(euler, OR, B, C); }
elar(A) ::= elar(B) NOT OR elar(C). { A = ast_add_node(euler, NOR, B, C); }
elar(A) ::= elar(B) XOR elar(C). {A = ast_add_node(euler, XOR, B, C);}
elar(A) ::= elar(B) GREATER GREATER elar(C). { A = ast_add_node(euler, SHIFTR, B, C); }
elar(A) ::= elar(B) SMALLER SMALLER elar(C). { A = ast_add_node(euler, SHIFTL, B, C); }
elar(A) ::= NOT elar(B). { A = ast_add_node(euler, NOT, B, NULL); }

/* Binary Relations */
elar(A) ::= elar(B) BAND elar(C). {A = ast_add_node(euler, BAND, B, C); }
elar(A) ::= elar(B) BOR elar(C). { A = ast_add_node(euler, BOR, B, C); }
elar(A) ::= elar(B) ISEQ elar(C). { A = ast_add_node(euler, ISEQ, B, C); }
elar(A) ::= elar(B) GREATER elar(C). { A = ast_add_node(euler, GREATER, B, C); }
elar(A) ::= elar(B) SMALLER elar(C). { A = ast_add_node(euler, SMALLER, B, C); }

/* Elementary Arithmetic */
elar(A) ::= LETTER. { A = ast_add_leaf(euler, LETTER); }
elar(A) ::= number(B).                    { A = B; }
elar(A) ::= MINUS elar(B).                { A = ast_add_node(euler, MINUS, B, NULL); }
elar(A) ::= PLUS elar(B).                 { A = ast_add_node(euler, PLUS, B, NULL);      }
elar(A) ::= LPAREN elar(B) RPAREN.        { A = B;       }
elar(A) ::= elar(B)    PLUS      elar(C). { A = ast_add_node(euler, PLUS, B, C);   }
 elar(A) ::= elar(B)    MINUS     elar(C). { A = ast_add_node(euler, MINUS, B, C);   }
elar(A) ::= elar(B)    MULT      elar(C). {  A = ast_add_node(euler, MULT, B, C);  }
elar(A) ::= elar(B)    DIV       elar(C). {  A = ast_add_node(euler, DIV, B, C); }
elar(A) ::= elar(B)    EXP       elar(C). {  A = ast_add_node(euler, EXP, B, C); }
elar(A) ::= elar(B)    MOD       elar(C). {  A = ast_add_node(euler, MOD, B, C); }
elar(A) ::= elar(B)    FACT.              {  A = ast_add_node(euler, FACT, B, NULL); }

number(A) ::= FLOAT.                     { A = ast_add_leaf(euler, FLOAT); }
number(A) ::= INT.                       { A = ast_add_leaf(euler, INT); }
 number(A) ::= constant(B).                  { A = B;  }

constant(A) ::= PI. { A = ast_add_leaf_const(euler, CONST, _PI);  }
constant(A) ::= E. { A = ast_add_leaf_const(euler, CONST, _E); }
