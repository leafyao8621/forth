#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_begin(ForthParser *parser, ForthVM *vm) {
    if (parser->state & PARSER_STATE_INTERPRET) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (parser->loop_stack_cur == parser->loop_stack_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_OVERFLOW;
    }
    *(parser->loop_stack_cur++) = PARSER_CONTROL_BEGIN;
    *(uint8_t**)parser->loop_stack_cur = vm->compiled_cur;
    parser->loop_stack_cur += sizeof(uintptr_t);
    return PARSER_STATUS_OK;
}
