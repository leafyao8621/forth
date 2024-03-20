#include <fsi/util/errcode.h>

const char *fsi_errcode_lookup[10] =
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
        "FORTHVM_ERR_RETURN_STACK_EMPTY"
    };
