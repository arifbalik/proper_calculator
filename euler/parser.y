%include {
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "euler.h"
#include "variables.h"
}

%token_type { struct token_info }

%code {

}

%nonassoc EQ.
%left PLUS MINUS.
%left MULT DIV.

%parse_accept { }

%syntax_error { printf("Syntax Error!\n"); }

%parse_failure { printf("parser failed!\n") }

%start_symbol program

program ::= line.

line ::= expression(C). {
        printf("%f\n", C.val);
        C.val = 0;
}

expression ::= LIST. { list(); }
expression(C) ::= NAME(A). { C.val = get_var_val(A.name); }
expression(C) ::= expression(B) EQ NAME(A). { C.val = A.val = add(A.name, B.val); }

expression(C) ::= expression(A) PLUS expression(B). { C.val = A.val + B.val; }
expression(C) ::= expression(A) MINUS expression(B). { C.val = A.val - B.val;  }
expression(C) ::= expression(A) MULT expression(B). { C.val = A.val * B.val; }
expression(C) ::= expression(A) DIV expression(B). {
        if(B.val != 0)
                C.val = A.val / B.val;
        else
                printf("Math Error!");
}

expression(C) ::= number(B). {C.val = B.val;}
number ::= INT.
number ::= FLOAT.












expression ::= DUMMY.
