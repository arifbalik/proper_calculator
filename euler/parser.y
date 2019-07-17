%token_type { int }

%include {
#include <assert.h>
#include <stdio.h>
int total = 0;
int total_mult = 1;
}

%code{
  
}


%left PLUS MINUS.
%left MULT DIV.

%parse_accept { }

%syntax_error { fprintf(stderr, "Syntax Error!\n"); }

%parse_failure {  }

%start_symbol program

number ::= INT.
number ::= FLOAT.

expression ::= number.
expression(C) ::= expression(A) PLUS expression(B). { C = A + B; }
expression(C) ::= expression(A) MINUS expression(B). { C = A - B;  }
expression(C) ::= expression(A) MULT expression(B). { C = A * B; }
expression(C) ::= expression(A) DIV expression(B). { C = A / B;  }


line ::= expression(C). {printf("%d\n", C); total = 0;} 

program ::= line.
