#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_until(ForthParser *parser, ForthVM *vm) {
    if (parser->state & PARSER_STATE_INTERPRET) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (
        parser->loop_stack_cur - (sizeof(uintptr_t) + 1) <
        parser->loop_stack) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_UNDERFLOW;
    }
    parser->loop_stack_cur -= (sizeof(uintptr_t) + 1);
    if (*parser->loop_stack_cur != PARSER_CONTROL_BEGIN) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_MISMATCH;
    }
    if (vm->compiled_cur == vm->compiled_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm->compiled_cur++) = VM_INSTRUCTION_JZD;
    *(uint8_t**)(vm->compiled_cur) =
        *(uint8_t**)(parser->loop_stack_cur + 1) - 1;
    if (vm->compiled_cur + sizeof(uintptr_t) > vm->compiled_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    vm->compiled_cur += sizeof(uintptr_t);
    return PARSER_STATUS_OK;
}
