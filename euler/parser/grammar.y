%include {
#include <assert.h>
#include "tokenizer.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wredundant-decls"
#pragma GCC diagnostic ignored "-Wstrict-prototypes"
#pragma GCC diagnostic ignored "-Wmissing-prototypes"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wsign-compare"


}

%type elar { __double }
%extra_argument { ersl_t *euler }
%start_symbol   query
%stack_overflow {printf("stack_overflow\n");}
%parse_failure {printf("parser failed\n");}

/* Priorities */
%nonassoc EQ.
%left PLUS MINUS.
%left MULT DIV.
%left LAPREN RPAREN.

%wildcard ANY.

/* Elementart Arithmetic */
query ::= elar.

/* Basic Algebra Arithmetic */
elar(A) ::= INT(B).                       { A = B;       }
elar(A) ::= FLOAT(B).                     { A = B;       }
elar(A) ::= CONST(B).                     { A = B;       }
elar(A) ::= MINUS elar(B).                { A = -B;      }
elar(A) ::= PLUS elar(B).                 { A = B;       }
elar(A) ::= elar(B)    PLUS      elar(C). { A = B + C;   }
elar(A) ::= elar(B)    MINUS     elar(C). { A = B - C;   }
elar(A) ::= elar(B)    MULT      elar(C). { A = B * C;   }
elar(A) ::= elar(B)    DIV       elar(C). { A = (C != 0) ? (B / C) : (euler->status = MTHE); }



%code {
#pragma GCC diagnostic pop
}
