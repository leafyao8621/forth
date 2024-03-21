#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <fsi/interpreter/vm.h>

#define FORTHPARSER_STATE_INTERPRET 0
#define FORTHPARSER_STATE_COMPILE 1
#define FORTHPARSER_STATE_DEFINE 2

typedef struct ForthParser {
    char *str;
    char *iter;
    char state;
    char base;
    size_t offset;
    String token_buf;
} ForthParser;

ForthVMErr ForthParser_initialize(ForthParser *parser);
void ForthParser_finalize(ForthParser *parser);
ForthVMErr ForthParser_parse(ForthParser *parser, char *str, ForthVM *vm);

#endif
