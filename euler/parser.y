%include {
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "euler.h"

struct symbol_table stbl[50];
static int vrc = 0;



void list(void){
        int i = vrc;
        if(i < 0) return;
        while(i--){
                printf("Name: %s, value: %f\n", stbl[i].name, stbl[i].val);
        }
}
int lookup(char *name){
        int i = vrc - 1;
        if(i < 0) return 0;
        while(strcmp(name, (stbl + i)->name) != 0){
                i--;
                if (i < 0){
                        return -1;
                }
        }
        return i;
}

double add(char *name, double val){
        
        if(lookup(name) != -1 && vrc > 0){
                printf("Given Name: %s, index: %d, Found name:%s val:%f\n", name, lookup(name), (stbl + lookup(name))->name, (stbl + lookup(name))->val);
                (stbl + lookup(name))->val = val;
                printf("Updated Value: %f\n", (stbl + lookup(name))->val);
                return val;
        }
        
        (stbl + vrc)->name = (char *)malloc(sizeof(char) * MAX_VARNAME_LEN);
        strcpy((stbl + vrc)->name, name);
        (stbl + vrc)->val = val;
        vrc++;
}


}

%token_type { struct token_info }

%code{
        
}

%nonassoc EQ.
%left PLUS MINUS.
%left MULT DIV.

%parse_accept { }

%syntax_error { fprintf(stderr, "Syntax Error!\n"); }

%parse_failure {  }

%start_symbol program

program ::= line.

line ::= expression(C). {printf("%f\n", C.val);
        C.val = 0;
        strcpy(C.name, " ");
}

expression ::= LIST. { list(); }
expression(C) ::= var(A). {
        strcpy(C.name, A.name);
        C.val = (stbl + lookup(A.name))->val;
        printf("Name: %s\n", A.name);
}

expression(C) ::= expression(B) EQ var(A). {
        printf("asignee: %s\n", A.name);
        printf("expression: %f\n", B.val);
        C.val = (stbl + lookup(A.name))->val = add(A.name, B.val);
}

var ::= NAME.

expression(C) ::= expression(A) PLUS expression(B). { C.val = A.val + B.val; }
expression(C) ::= expression(A) MINUS expression(B). { C.val = A.val - B.val;  }
expression(C) ::= expression(A) MULT expression(B). { C.val = A.val * B.val; }
expression(C) ::= expression(A) DIV expression(B).      { 
                                                                if(B.val != 0)
                                                                        C.val = A.val / B.val;
                                                                else
                                                                        printf("Math Error!"); 
                                                        }

expression(C) ::= number(B). {C.val = B.val;}
number ::= INT.
number ::= FLOAT.












expression ::= DUMMY.





