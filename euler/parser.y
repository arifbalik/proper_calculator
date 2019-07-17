%include {
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "euler.h"

struct symbol_table stbl[50];
static int vrc = 0;

double add(char *name, double val){
        (stbl + vrc)->name = (char *)malloc(sizeof(char) * MAX_VARNAME_LEN);
        strcpy((stbl + vrc)->name, name);
        (stbl + vrc)->val = val;
        vrc++;
}

double lookup(char *name){
        int i = vrc - 1;
        while(strcmp(name, (stbl + i)->name) != 0){
                i--;
        } 

        return (stbl + i)->val;
}
}

%token_type { struct token_info }

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
expression(C) ::= NAME(A). {C.val = lookup(A.name);}
expression(C) ::= expression(A) PLUS expression(B). { C.val = A.val + B.val; }
expression(C) ::= expression(A) MINUS expression(B). { C.val = A.val - B.val;  }
expression(C) ::= expression(A) MULT expression(B). { C.val = A.val * B.val; }
expression(C) ::= expression(A) DIV expression(B).      { 
                                                                if(B.val != 0)
                                                                        C.val = A.val / B.val;
                                                                else
                                                                        printf("Math Error!"); 
                                                        }
expression(C) ::= NAME(A) EQ number(B). {
        C.val = add(A.name, B.val);
        
}
expression ::= DUMMY.


line ::= expression(C). {printf("%f\n", C.val);} 

program ::= line.
