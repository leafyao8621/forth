#include "src\parser\parser.h"
#include "src\vm\vm.h"
#include "src\util\status.h"

int parser_handler_then(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (parser_conditional_stack_cur == parser_conditional_stack) {
        return PARSER_STATUS_PARSER_CONTROL_STACK_UNDERFLOW;
    }
    parser_conditional_stack_cur -= (sizeof(uintptr_t) + 1);
    if (
        *parser_conditional_stack_cur != PARSER_CONTROL_IF &&
        *parser_conditional_stack_cur != PARSER_CONTROL_ELSE) {
        return PARSER_STATUS_PARSER_CONTROL_STACK_MISMATCH;
    }
    **(uint8_t***)(parser_conditional_stack_cur + 1) = vm_compiled_cur - 1;
    return PARSER_STATUS_OK;
}
