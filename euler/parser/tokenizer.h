#ifndef __LEXER__
#define __LEXER__

#include "../inc/euler.h"

/* Just change of notation */
#define parse Parse
#define parse_free ParseFree
#define parse_alloc ParseAlloc

/* Priority Flags */
#define __no_priority 0
#define __func_begin 1
#define __func_end 2

void *ParseAlloc(void);
void ParseTrace(FILE *TraceFILE, char *zTracePrompt);
void Parse(void *, int, ast_t *, ersl_t *);
void ParseFree(void *);
int lex(symbol_table_t *symbol_table);
void fill_lex(char *q);

#endif /* __LEXER__ */
