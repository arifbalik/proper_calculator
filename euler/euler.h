/*
        File info
 */
#ifndef __EULER__
#define __EULER__

#define MAX_QUERY_SIZE 20
#define MAX_VARNAME_LEN 10
#define EOL 0
#define QUIT -2

void *ParseAlloc();
void Parse();
void ParseFree();


struct token_info{
        char *name;
        double val;
};


struct symbol_table{
    char *name;
    double val;
};

static int enc = 0;


#endif /* __EULER__ */
