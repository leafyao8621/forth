#include "status.h"

const char *parser_status_lookup[11] =
    {
        "PARSER_STATUS_OK",
        "PARSER_STATUS_END_REPL",
        "PARSER_STATUS_TOKEN_TOO_LONG",
        "PARSER_STATUS_INTERPRETED_OVERFLOW",
        "PARSER_STATUS_COMPILED_OVERFLOW",
        "PARSER_STATUS_INVALID_BASE",
        "PARSER_STATUS_INVALID_BASE10",
        "PARSER_STATUS_NESTED_COMPILATION_MODE",
        "PARSER_STATUS_NOT_IN_COMPILATION_MODE",
        "PARSER_STATUS_LOOKUP_OVERFLOW",
        "PARSER_STATUS_PENDING_DEFINITION"
    };

const char *vm_status_lookup[6] =
    {
        "VM_STATUS_OK",
        "VM_STATUS_DATA_STACK_OVERFLOW",
        "VM_STATUS_DATA_STACK_UNDERFLOW",
        "VM_STATUS_INVALID_BASE",
        "VM_STATUS_CONTROL_STACK_OVERFLOW",
        "VM_STATUS_CONTROL_STACK_UNDERFLOW"
    };
