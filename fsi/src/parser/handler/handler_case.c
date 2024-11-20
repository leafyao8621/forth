#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_case(ForthParser *parser, ForthVM *vm) {
    if (parser->state & PARSER_STATE_INTERPRET) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (parser->case_stack_cur == parser->case_stack_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_OVERFLOW;
    }
    *(parser->case_stack_cur++) = PARSER_CONTROL_CASE;
    *(uint8_t**)parser->case_stack_cur = vm->compiled_cur;
    parser->case_stack_cur += sizeof(uintptr_t);
    return PARSER_STATUS_OK;
}
