#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_endcase(ForthParser *parser, ForthVM *vm) {
    if (parser->state & PARSER_STATE_INTERPRET) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (parser->case_stack_cur == parser->case_stack) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_UNDERFLOW;
    }
    parser->case_stack_cur -= (sizeof(uintptr_t) + 1);
    if (
        *parser->case_stack_cur != PARSER_CONTROL_CASE &&
        *parser->case_stack_cur != PARSER_CONTROL_ENDOF) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_MISMATCH;
    }
    for (
        ;
        parser->case_stack_cur >= parser->case_stack &&
        *parser->case_stack_cur == PARSER_CONTROL_ENDOF;
        parser->case_stack_cur -= (sizeof(uintptr_t) + 1)) {
        **(uint8_t***)(parser->case_stack_cur + 1) =
            vm->compiled_cur - 1;
    }
    if (*parser->case_stack_cur != PARSER_CONTROL_CASE) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_MISMATCH;
    }
    return PARSER_STATUS_OK;
}
