#include "../parser.h"
#include "../../vm/vm.h"
#include "../../util/status.h"

int parser_handler_cr(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_PUSHD;
        if (vm_interpreted_cur + sizeof(uintptr_t) > vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(uintptr_t*)vm_interpreted_cur = 10;
        vm_interpreted_cur += sizeof(uintptr_t);
        if (vm_interpreted_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm_interpreted_cur++) = VM_INSTRUCTION_EMIT;
    }
    if (parser_state & PARSER_STATE_COMPILE) {
        if (vm_compiled_cur == vm_interpreted_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_PUSHD;
        if (vm_compiled_cur + sizeof(uintptr_t) >= vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(uintptr_t*)vm_compiled_cur = 10;
        vm_compiled_cur += sizeof(uintptr_t);
        if (vm_compiled_cur == vm_compiled_end) {
            parser_status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm_compiled_cur++) = VM_INSTRUCTION_EMIT;
    }
    return PARSER_STATUS_OK;
}
