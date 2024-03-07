#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <fsi/interpreter/vm.h>

typedef struct ForthParser {
    char *str;
    char *iter;
    bool compile;
    char base;
    String token_buf;
} ForthParser;

ForthVMErr ForthParser_initialize(ForthParser *parser);
void ForthParser_finalize(ForthParser *parser);
ForthVMErr ForthParser_parse(ForthParser *parser, char *str, ForthVM *vm);

#endif
