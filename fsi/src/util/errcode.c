#include <fsi/util/errcode.h>

const char *fsi_errcode_lookup[15] =
    {
        "FORTHVM_ERR_OK",
        "FORTHVM_ERR_NULL_PTR",
        "FORTHVM_ERR_OUT_OF_MEMORY",
        "FORTHVM_ERR_FILE_IO",
        "FORTHVM_ERR_QUOTE",
        "FORTHVM_ERR_INVALID_DECIMAL",
        "FORTHVM_ERR_INVALID_BASE",
        "FORTHVM_ERR_STACK_UNDERFLOW",
        "FORTHVM_ERR_NOT_IN_COMPILATION_MODE",
        "FORTHVM_ERR_RETURN_STACK_EMPTY",
        "FORTHVM_ERR_PENDING_DEFINITION",
        "FORTHVM_ERR_NOT_BALANCED",
        "FORTHVM_ERR_NOT_IN_INTERPRETATION_MODE",
        "FORTHVM_ERR_ILLEGAL_ACCESS",
        "FORTHVM_ERR_MEMORY_UNDERFLOW"
    };
