#include "src\parser\parser.h"
#include "src\vm\vm.h"
#include "src\util\status.h"

int parser_handler_if(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_JZD;
    if (parser_conditional_stack_cur == parser_conditional_stack_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_OVERFLOW;
    }
    *(parser_conditional_stack_cur++) = PARSER_CONTROL_IF;
    *(uint8_t**)parser_conditional_stack_cur = vm_compiled_cur;
    parser_conditional_stack_cur += sizeof(uintptr_t);
    if (vm_compiled_cur + sizeof(uintptr_t) > vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    vm_compiled_cur += sizeof(uintptr_t);
    return PARSER_STATUS_OK;
}
