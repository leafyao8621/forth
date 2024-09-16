#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <containers/dstring.h>

#include <fsi/util/status.h>
#include <fsi/vm/vm.h>

#define PARSER_STATE_INTERPRET 0x1
#define PARSER_STATE_COMPILE 0x2
#define PARSER_STATE_NAME 0x4
#define PARSER_STATE_CREATE 0x8

#define PARSER_STATUS_RUNNING 0
#define PARSER_STATUS_END 1
#define PARSER_STATUS_REPL_END 2

#define PARSER_CONTROL_IF 0
#define PARSER_CONTROL_ELSE 1
#define PARSER_CONTROL_DO 2
#define PARSER_CONTROL_DO_LEAVE 3
#define PARSER_CONTROL_BEGIN 4
#define PARSER_CONTROL_WHILE 5

typedef struct ForthParser {
    String buf;
    uint8_t state;
    uint8_t status;
    uint8_t *pending;
    uint8_t conditional_stack[(sizeof(uintptr_t) + 1) * 20];
    uint8_t *conditional_stack_cur;
    uint8_t *conditional_stack_end;
    uint8_t loop_stack[(sizeof(uintptr_t) + 1) * 20];
    uint8_t *loop_stack_cur;
    uint8_t *loop_stack_end;
} ForthParser;

ForthParserStatus parser_initialize(ForthParser *parser);
void parser_finalize(ForthParser *parser);
ForthParserStatus parser_parse(
    ForthVM *vm, ForthParser *parser, bool debug, bool line, char *str);

#endif
