#include "src\parser\parser.h"
#include "src\vm\vm.h"
#include "src\util\status.h"

int parser_handler_do(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_2PUSHC;
    if (parser_loop_stack_cur == parser_loop_stack_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_OVERFLOW;
    }
    *(parser_loop_stack_cur++) = PARSER_CONTROL_DO;
    *(uint8_t**)parser_loop_stack_cur = vm_compiled_cur;
    parser_loop_stack_cur += sizeof(uintptr_t);
    return PARSER_STATUS_OK;
}
