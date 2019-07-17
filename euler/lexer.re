
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

void *ParseAlloc();
void Parse();
void ParseFree();

#define MAX_QUERY_SIZE 20
#define EOL 0

char* YYCURSOR;
void *parser;
char* start;

struct symbol_table{
    int number;
    char *string;
    int token;
}*symtbl;

struct symbol_table *fill_symtbl(double number, char *string, int token){
    symtbl->number = number;
    symtbl->string = string;
    symtbl->token = token;
    return symtbl;
}

static struct symbol_table* lex(char *buff_end){

    const char* YYMARKER;

    if(YYCURSOR >= buff_end){
        return fill_symtbl(0,0,EOL);
    } 
    /*!re2c
        re2c:define:YYCTYPE = char;
        re2c:yyfill:enable = 0;

        int = [-+]?[0-9]+;
        float = [-+]?[0-9]+('.'[0-9]+)('e'[+-]?[0-9]+)? ;

        int {
                return fill_symtbl(atoi(strndup(start,YYCURSOR-start)),0,INT);
            }
        float {return fill_symtbl(0,0,FLOAT);}
        "p" { return fill_symtbl(0,0,PLUS); }
        "m" { return fill_symtbl(0,0,MINUS); }
        "*" { return fill_symtbl(0,0,MULT); }
        "/" { return fill_symtbl(0,0,DIV); }
        *   { return ;fill_symtbl(0,0,-2); }
    */

    
}

int main(int argc, char **argv){

    
    char *buff, *buff_end;
    
    symtbl = malloc(sizeof(struct symbol_table));
    fill_symtbl(0,0,-1);

    buff = (char*) malloc(MAX_QUERY_SIZE * sizeof(char));
    parser = ParseAlloc( malloc );
    
    while(1){

        printf(">>>");
        scanf("%s",buff);
        buff_end =  (char*) (((char*)buff) + strlen(buff));
        YYCURSOR = buff;
        start = YYCURSOR;
        while(symtbl->token != EOL){
            lex(buff_end);
            if(symtbl->token == -2) goto end;
            Parse(parser, symtbl->token, symtbl->number);
            start = YYCURSOR;
        }
        symtbl->token = -1;
    }

    end:
    ParseFree(free);
    free(buff);
    return(0);
}