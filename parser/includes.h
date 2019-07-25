/*
        File info
 */
#ifndef __EULER__
#define __EULER__

#define MAX_QUERY_SIZE 20
#define MAX_VAR_QTY 10
#define MAX_VARNAME_LEN 10

#define EOL 0
#define QUIT -2

void *ParseAlloc();
void Parse();
void ParseFree();
int cmd(char *query);

extern int __err;

typedef struct {
	char name[MAX_VARNAME_LEN];
	double val;
} token_info;

typedef struct {
	char name[MAX_VARNAME_LEN];
	double val;
} symbol_table;

#endif /* __EULER__ */
