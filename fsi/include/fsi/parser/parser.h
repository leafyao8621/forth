#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <fsi/interpreter/vm.h>

#define FORTHPARSER_STATE_INTERPRET 0
#define FORTHPARSER_STATE_COMPILE 1
#define FORTHPARSER_STATE_DEFINE 2
#define FORTHPARSER_STATE_VARIABLE 3
#define FORTHPARSER_STATE_CREATE 4

#define FORTHPARSER_CONDITIONAL_TYPE_IF 0
#define FORTHPARSER_CONDITIONAL_TYPE_ELSE 1
#define FORTHPARSER_LOOP_TYPE_DO 0
#define FORTHPARSER_LOOP_TYPE_BEGIN 1
#define FORTHPARSER_LOOP_TYPE_WHILE 2

typedef struct ForthParser {
    char *str;
    char *iter;
    char state, prev_state;
    bool deferred;
    size_t deferred_offset;
    char base;
    size_t offset;
    String token_buf;
    DArrayOffset conditional_offset;
    String conditional_type;
    DArrayOffset loop_offset;
    String loop_type;
} ForthParser;

ForthVMErr ForthParser_initialize(ForthParser *parser);
void ForthParser_finalize(ForthParser *parser);
ForthVMErr ForthParser_parse(ForthParser *parser, char *str, ForthVM *vm);

#endif
