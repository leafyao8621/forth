#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_else(ForthParser *parser, ForthVM *vm) {
    if (parser->state & PARSER_STATE_INTERPRET) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (parser->conditional_stack_cur == parser->conditional_stack) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_UNDERFLOW;
    }
    parser->conditional_stack_cur -= (sizeof(uintptr_t) + 1);
    if (
        *parser->conditional_stack_cur != PARSER_CONTROL_IF) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_MISMATCH;
    }
    *parser->conditional_stack_cur = PARSER_CONTROL_ELSE;
    if (vm->compiled_cur == vm->compiled_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm->compiled_cur++) = VM_INSTRUCTION_JMP;
    if (parser->conditional_stack_cur == parser->conditional_stack_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_OVERFLOW;
    }
    **(uint8_t***)(parser->conditional_stack_cur + 1) =
        vm->compiled_cur + sizeof(uintptr_t) - 1;
    *(uint8_t**)(parser->conditional_stack_cur + 1) = vm->compiled_cur;
    vm->compiled_cur += sizeof(uintptr_t);
    parser->conditional_stack_cur += (sizeof(uintptr_t) + 1);
    return PARSER_STATUS_OK;
}
