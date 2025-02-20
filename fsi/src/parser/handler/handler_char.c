#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_char(ForthParser *parser) {
    parser->state |= PARSER_STATE_CHAR;
    return PARSER_STATUS_OK;
}
