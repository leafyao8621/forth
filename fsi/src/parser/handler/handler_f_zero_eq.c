#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_f_zero_eq(ForthParser *parser, ForthVM *vm) {
    if (parser->state & PARSER_STATE_INTERPRET) {
        if (vm->interpreted_cur == vm->interpreted_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm->interpreted_cur++) = VM_INSTRUCTION_EQZF;
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        if (vm->compiled_cur == vm->compiled_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm->compiled_cur++) = VM_INSTRUCTION_EQZF;
    }
    return PARSER_STATUS_OK;
}
