#include "../parser.h"
#include "../../vm/vm.h"
#include "../../util/status.h"

int parser_handler_while(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (parser_loop_stack_cur == parser_loop_stack) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_UNDERFLOW;
    }
    if (
        *(parser_loop_stack_cur - sizeof(uintptr_t) - 1) !=
        PARSER_CONTROL_BEGIN) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_UNDERFLOW;
    }
    if (parser_loop_stack_cur == parser_loop_stack_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_OVERFLOW;
    }
    *(parser_loop_stack_cur++) = PARSER_CONTROL_WHILE;
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_JZD;
    if (parser_loop_stack_cur + sizeof(uintptr_t) > parser_loop_stack_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_OVERFLOW;
    }
    *(uint8_t**)parser_loop_stack_cur = vm_compiled_cur;
    parser_loop_stack_cur += sizeof(uintptr_t);
    if (vm_compiled_cur + sizeof(uintptr_t) > vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    vm_compiled_cur += sizeof(uintptr_t);
    return PARSER_STATUS_OK;
}
