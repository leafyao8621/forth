#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_create(ForthParser *parser, ForthVM *vm) {
    if (parser->state & PARSER_STATE_INTERPRET) {
        parser->state |= PARSER_STATE_CREATE;
        if (vm->interpreted_cur == vm->interpreted_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm->interpreted_cur++) = VM_INSTRUCTION_DEF;
    } else {
        *parser->pending |= VM_LOOKUP_META_MEMORY;
    }
    return PARSER_STATUS_OK;
}
