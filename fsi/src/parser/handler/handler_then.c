#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_then(ForthParser *parser, ForthVM *vm) {
    if (parser->state & PARSER_STATE_INTERPRET) {
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (parser->conditional_stack_cur == parser->conditional_stack) {
        return PARSER_STATUS_PARSER_CONTROL_STACK_UNDERFLOW;
    }
    parser->conditional_stack_cur -= (sizeof(uintptr_t) + 1);
    if (
        *parser->conditional_stack_cur != PARSER_CONTROL_IF &&
        *parser->conditional_stack_cur != PARSER_CONTROL_ELSE) {
        return PARSER_STATUS_PARSER_CONTROL_STACK_MISMATCH;
    }
    **(uint8_t***)(parser->conditional_stack_cur + 1) = vm->compiled_cur - 1;
    return PARSER_STATUS_OK;
}
