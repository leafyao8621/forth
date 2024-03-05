#ifndef UTIL_ERRCODE_H_
#define UTIL_ERRCODE_H_

typedef enum ForthVMErr {
    FORTHVM_ERR_OK,
    FORTHVM_ERR_NULL_PTR,
    FORTHVM_ERR_OUT_OF_MEMORY,
    FORTHVM_ERR_FILE_IO
} ForthVMErr;

extern const char *fsi_errcode_lookup[4];

#endif
