#include "../parser.h"
#include "../../vm/vm.h"
#include "../../util/status.h"

int parser_handler_create(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_state |= PARSER_STATE_CREATE;
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_DEF;
    } else {
        *parser_pending |= VM_LOOKUP_META_MEMORY;
    }
    return PARSER_STATUS_OK;
}
