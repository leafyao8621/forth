#include "../parser.h"
#include "../../vm/vm.h"
#include "../../util/status.h"

int parser_handler_then(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (parser_control_stack_cur == parser_control_stack) {
        return PARSER_STATUS_PARSER_CONTROL_STACK_UNDERFLOW;
    }
    parser_control_stack_cur -= (sizeof(uintptr_t) + 1);
    if (
        *parser_control_stack_cur != PARSER_CONTROL_IF &&
        *parser_control_stack_cur != PARSER_CONTROL_ELSE) {
        return PARSER_STATUS_PARSER_CONTROL_STACK_MISMATCH;
    }
    **(uint8_t***)(parser_control_stack_cur + 1) = vm_compiled_cur - 1;
    return PARSER_STATUS_OK;
}
