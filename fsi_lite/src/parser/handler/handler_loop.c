#include "../parser.h"
#include "../../vm/vm.h"
#include "../../util/status.h"

int parser_handler_loop(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (parser_loop_stack_cur == parser_loop_stack) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_UNDERFLOW;
    }
    parser_loop_stack_cur -= (sizeof(uintptr_t) + 1);
    if (
        *parser_loop_stack_cur != PARSER_CONTROL_DO &&
        *parser_loop_stack_cur != PARSER_CONTROL_DO_LEAVE) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_MISMATCH;
    }
    for (
        ;
        parser_loop_stack_cur >= parser_loop_stack &&
        *parser_loop_stack_cur == PARSER_CONTROL_DO_LEAVE;
        parser_loop_stack_cur -= (sizeof(uintptr_t) + 1)) {
        **(uint8_t***)(parser_loop_stack_cur + 1) =
            vm_compiled_cur + sizeof(uintptr_t) + 1;
    }
    if (*parser_loop_stack_cur != PARSER_CONTROL_DO) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_MISMATCH;
    }
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_INCC;
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_JNEC;
    *(uint8_t**)(vm_compiled_cur) =
        *(uint8_t**)(parser_loop_stack_cur + 1) - 1;
    if (vm_compiled_cur + sizeof(uintptr_t) > vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    vm_compiled_cur += sizeof(uintptr_t);
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_2RMC;
    return PARSER_STATUS_OK;
}
