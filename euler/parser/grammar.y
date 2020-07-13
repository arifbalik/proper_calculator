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
%start_symbol   expression
%stack_overflow {printf("stack_overflow\n");}
%parse_failure {printf("parser failed\n");}
%parse_failure {printf("syntax error\n");}

/* Priorities */
%nonassoc EQ INT FLOAT UNKNOWN TO SIGMA PROD PI E COMMA SHIFTL SHIFTR NAND NOR SHIFT_LEFT SHIFT_RIGHT.
%left CONST.
%left LETTER.
%left PLUS MINUS.
%left MULT DIV MOD FACT.
%left EXP.
%left AND OR NOT GREATER SMALLER XOR BAND BOR ISEQ.
%left LPAREN RPAREN.

%default_destructor {
        euler->status = 255;
}

%wildcard ANY.

expression ::= query EOQ. {
        ast_finalize(euler);
       // ast_print(ast_get_root(euler));
        ast_init(euler);
}

/* Elementary Arithmetic */
query ::= basic_arithmetic.
query ::= number.
query ::= algebra.
query ::= relation.
query ::= binary.



/* Boolean Operators */
binary(A) ::= query(B) AND query(C). {A = ast_add_node( AND, B, C); }
binary(A) ::= query(B) NOT AND query(C). { A = ast_add_node( NAND, B, C);  }
binary(A) ::= query(B) OR query(C). { A = ast_add_node( OR, B, C); }
binary(A) ::= query(B) NOT OR query(C). { A = ast_add_node( NOR, B, C); }
binary(A) ::= query(B) XOR query(C). {A = ast_add_node( XOR, B, C);}
binary(A) ::= query(B) GREATER GREATER query(C). { A = ast_add_node( SHIFTR, B, C); }
binary(A) ::= query(B) SMALLER SMALLER query(C). { A = ast_add_node( SHIFTL, B, C); }
binary(A) ::= NOT query(B). { A = ast_add_node( NOT, B, NULL); }

/* Binary Relations */
relation(A) ::= query(B) BAND query(C). {A = ast_add_node( BAND, B, C); }
relation(A) ::= query(B) BOR query(C). { A = ast_add_node( BOR, B, C); }
relation(A) ::= query(B) ISEQ query(C). { A = ast_add_node( ISEQ, B, C); }
relation(A) ::= query(B) GREATER query(C). { A = ast_add_node( GREATER, B, C); }
relation(A) ::= query(B) SMALLER query(C). { A = ast_add_node( SMALLER, B, C); }

/* Algebra */
algebra(A) ::= LETTER. { A = ast_add_leaf( LETTER); }

/* Elementary Arithmetic */
basic_arithmetic(A) ::= MINUS query(B).                { A = ast_add_node( MINUS, B, NULL); }
basic_arithmetic(A) ::= PLUS query(B).                 { A = ast_add_node( PLUS, B, NULL);      }
basic_arithmetic(A) ::= LPAREN query(B) RPAREN.        { A = B;       }
basic_arithmetic(A) ::= query(B)    PLUS      query(C). { A = ast_add_node( PLUS, B, C);   }
basic_arithmetic(A) ::= query(B)    MINUS     query(C). { A = ast_add_node( MINUS, B, C);   }
basic_arithmetic(A) ::= query(B)    MULT      query(C). {  A = ast_add_node( MULT, B, C);  }
basic_arithmetic(A) ::= query(B)    DIV       query(C). {  A = ast_add_node( DIV, B, C); }
basic_arithmetic(A) ::= query(B)    EXP       query(C). {  A = ast_add_node( EXP, B, C); }
basic_arithmetic(A) ::= query(B)    MOD       query(C). {  A = ast_add_node( MOD, B, C); }
basic_arithmetic(A) ::= query(B)    FACT.              {  A = ast_add_node( FACT, B, NULL); }


number(A) ::= FLOAT.                     { A = ast_add_leaf( FLOAT); }
number(A) ::= INT.                       { A = ast_add_leaf( INT); }
number(A) ::= BINARY.                    { A = ast_add_leaf( BINARY); }
number(A) ::= constant(B).                  { A = B;  }

constant(A) ::= PI. { A = ast_add_leaf_const( CONST, _PI);  }
constant(A) ::= E. { A = ast_add_leaf_const( CONST, _E); }
