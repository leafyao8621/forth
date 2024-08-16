#include "../parser.h"
#include "../../vm/vm.h"
#include "../../util/status.h"

int parser_handler_semi_colon(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (
        parser_conditional_stack_cur != parser_conditional_stack ||
        parser_loop_stack_cur != parser_loop_stack) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_MISMATCH;
    }
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_RET;
    parser_state ^= PARSER_STATE_COMPILE;
    parser_state |= PARSER_STATE_INTERPRET;
    parser_pending = 0;
    return PARSER_STATUS_OK;
}
