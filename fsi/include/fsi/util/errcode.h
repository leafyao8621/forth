#ifndef UTIL_ERRCODE_H_
#define UTIL_ERRCODE_H_

typedef enum ForthVMErr {
    FORTHVM_ERR_OK,
    FORTHVM_ERR_NULL_PTR,
    FORTHVM_ERR_OUT_OF_MEMORY,
    FORTHVM_ERR_FILE_IO,
    FORTHVM_ERR_QUOTE,
    FORTHVM_ERR_INVALID_DECIMAL,
    FORTHVM_ERR_INVALID_BASE,
    FORTHVM_ERR_STACK_UNDERFLOW
} ForthVMErr;

extern const char *fsi_errcode_lookup[8];

#endif
