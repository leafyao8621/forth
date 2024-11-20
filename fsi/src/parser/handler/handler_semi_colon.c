#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_semi_colon(ForthParser *parser, ForthVM *vm) {
    if (parser->state & PARSER_STATE_INTERPRET) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (
        parser->conditional_stack_cur != parser->conditional_stack ||
        parser->case_stack_cur != parser->case_stack ||
        parser->loop_stack_cur != parser->loop_stack) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_MISMATCH;
    }
    if (vm->compiled_cur == vm->compiled_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm->compiled_cur++) = VM_INSTRUCTION_RET;
    if (vm->compiled_cur == vm->compiled_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm->compiled_cur++) = VM_INSTRUCTION_HALT;
    parser->state ^= PARSER_STATE_COMPILE;
    parser->state |= PARSER_STATE_INTERPRET;
    parser->pending = 0;
    return PARSER_STATUS_OK;
}
