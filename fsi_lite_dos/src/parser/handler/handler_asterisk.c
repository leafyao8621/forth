#include "../parser.h"
#include "../../vm/vm.h"
#include "../../util/status.h"

int parser_handler_asterisk(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_MULTD;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_MULTD;
    }
    return PARSER_STATUS_OK;
}