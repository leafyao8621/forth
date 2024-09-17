#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_colon(ForthParser *parser) {
    if (parser->state & PARSER_STATE_COMPILE) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_NESTED_COMPILATION_MODE;
    }
    parser->state ^= PARSER_STATE_INTERPRET;
    parser->state |= PARSER_STATE_COMPILE;
    parser->state |= PARSER_STATE_NAME;
    return PARSER_STATUS_OK;
}
