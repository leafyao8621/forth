#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_comma(ForthParser *parser, ForthVM *vm) {
    if (parser->state & PARSER_STATE_COMPILE) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_INTERPRETATION_MODE;
    }
    if (vm->interpreted_cur == vm->interpreted_end) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_INTERPRETED_OVERFLOW;
    }
    *(vm->interpreted_cur++) = VM_INSTRUCTION_PUSHM;
    return PARSER_STATUS_OK;
}
