#include "status.h"

const char *parser_status_lookup[7] =
    {
        "PARSER_STATUS_OK",
        "PARSER_STATUS_END_REPL",
        "PARSER_STATUS_TOKEN_TOO_LONG",
        "PARSER_STATUS_INTERPRETED_OVERFLOW",
        "PARSER_STATUS_COMPILED_OVERFLOW",
        "PARSER_STATUS_INVALID_BASE",
        "PARSER_STATUS_INVALID_BASE10"
    };

const char *vm_status_lookup[4] =
    {
        "VM_STATUS_OK",
        "VM_STATUS_DATA_STACK_OVERFLOW",
        "VM_STATUS_DATA_STACK_UNDERFLOW",
        "VM_STATUS_INVALID_BASE"
    };
