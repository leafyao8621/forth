#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_while(ForthParser *parser, ForthVM *vm) {
    if (parser->state & PARSER_STATE_INTERPRET) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (parser->loop_stack_cur == parser->loop_stack) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_UNDERFLOW;
    }
    if (
        *(parser->loop_stack_cur - sizeof(uintptr_t) - 1) !=
        PARSER_CONTROL_BEGIN) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_UNDERFLOW;
    }
    if (parser->loop_stack_cur == parser->loop_stack_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_OVERFLOW;
    }
    *(parser->loop_stack_cur++) = PARSER_CONTROL_WHILE;
    if (vm->compiled_cur == vm->compiled_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm->compiled_cur++) = VM_INSTRUCTION_JZD;
    if (parser->loop_stack_cur + sizeof(uintptr_t) > parser->loop_stack_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_OVERFLOW;
    }
    *(uint8_t**)parser->loop_stack_cur = vm->compiled_cur;
    parser->loop_stack_cur += sizeof(uintptr_t);
    if (vm->compiled_cur + sizeof(uintptr_t) > vm->compiled_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    vm->compiled_cur += sizeof(uintptr_t);
    return PARSER_STATUS_OK;
}
