#include "status.h"

const char *parser_status_lookup[5] =
    {
        "PARSER_STATUS_OK",
        "PARSER_STATUS_END_REPL",
        "PARSER_STATUS_TOKEN_TOO_LONG"
        "PARSER_STATUS_INTERPRETED_OVERFLOW",
        "PARSER_STATUS_COMPILED_OVERFLOW"
    };
