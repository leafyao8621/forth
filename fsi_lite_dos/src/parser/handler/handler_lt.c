#include "src\parser\parser.h"
#include "src\vm\vm.h"
#include "src\util\status.h"

int parser_handler_lt(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_LTD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_LTD;
    }
    return PARSER_STATUS_OK;
}
