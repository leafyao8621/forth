#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_of(ForthParser *parser, ForthVM *vm) {
    if (parser->state & PARSER_STATE_INTERPRET) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (parser->case_stack_cur == parser->case_stack) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_UNDERFLOW;
    }
    if (
        *(uint8_t*)(parser->case_stack_cur - (sizeof(uintptr_t) + 1)) !=
        PARSER_CONTROL_CASE &&
        *(uint8_t*)(parser->case_stack_cur - (sizeof(uintptr_t) + 1)) !=
        PARSER_CONTROL_ENDOF) {
        printf("%hhX\n", *(uint8_t*)(parser->case_stack_cur - (sizeof(uintptr_t) + 1)));
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_MISMATCH;
    }
    if (vm->compiled_cur == vm->compiled_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm->compiled_cur++) = VM_INSTRUCTION_JNED;
    if (parser->case_stack_cur == parser->case_stack_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_OVERFLOW;
    }
    if (parser->case_stack_cur == parser->case_stack_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_OVERFLOW;
    }
    *(parser->case_stack_cur++) = PARSER_CONTROL_OF;
    if (
        parser->case_stack_cur + sizeof(uintptr_t) >
        parser->case_stack_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_OVERFLOW;
    }
    *(uint8_t**)parser->case_stack_cur = vm->compiled_cur;
    parser->case_stack_cur += sizeof(uintptr_t);
    if (vm->compiled_cur + sizeof(uintptr_t) > vm->compiled_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    vm->compiled_cur += sizeof(uintptr_t);
    return PARSER_STATUS_OK;
}
