#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <fsi/interpreter/vm.h>

#define FORTHPARSER_STATE_INTERPRET 0
#define FORTHPARSER_STATE_COMPILE 1
#define FORTHPARSER_STATE_DEFINE 2

#define FORTHPARSER_CONTROL_TYPE_IF 0
#define FORTHPARSER_CONTROL_TYPE_ELSE 1
#define FORTHPARSER_CONTROL_TYPE_DO 2
#define FORTHPARSER_CONTROL_TYPE_WHILE 3

typedef struct ForthParser {
    char *str;
    char *iter;
    char state;
    char base;
    size_t offset;
    String token_buf;
    DArrayOffset control_offset;
    String control_type;
} ForthParser;

ForthVMErr ForthParser_initialize(ForthParser *parser);
void ForthParser_finalize(ForthParser *parser);
ForthVMErr ForthParser_parse(ForthParser *parser, char *str, ForthVM *vm);

#endif
