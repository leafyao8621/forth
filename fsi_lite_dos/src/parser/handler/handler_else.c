#include "../parser.h"
#include "../../vm/vm.h"
#include "../../util/status.h"

int parser_handler_else(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (parser_conditional_stack_cur == parser_conditional_stack) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_UNDERFLOW;
    }
    parser_conditional_stack_cur -= (sizeof(uintptr_t) + 1);
    if (
        *parser_conditional_stack_cur != PARSER_CONTROL_IF) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_MISMATCH;
    }
    *parser_conditional_stack_cur = PARSER_CONTROL_ELSE;
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_JMP;
    if (parser_conditional_stack_cur == parser_conditional_stack_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_OVERFLOW;
    }
    **(uint8_t***)(parser_conditional_stack_cur + 1) =
        vm_compiled_cur + sizeof(uintptr_t) - 1;
    *(uint8_t**)(parser_conditional_stack_cur + 1) = vm_compiled_cur;
    vm_compiled_cur += sizeof(uintptr_t);
    parser_conditional_stack_cur += (sizeof(uintptr_t) + 1);
    return PARSER_STATUS_OK;
}
