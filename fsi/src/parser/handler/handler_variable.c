#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_variable(ForthParser *parser, ForthVM *vm) {
    if (parser->state & PARSER_STATE_INTERPRET) {
        parser->state |= PARSER_STATE_CREATE;
        if (vm->interpreted_cur == vm->interpreted_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm->interpreted_cur++) = VM_INSTRUCTION_DEFA;
    } else {
        *parser->pending |= VM_LOOKUP_META_CREATE;
        if (vm->compiled_cur == vm->compiled_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm->compiled_cur++) = VM_INSTRUCTION_ALLOCC;
    }
    return PARSER_STATUS_OK;
}
