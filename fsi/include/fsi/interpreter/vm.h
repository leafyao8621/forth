#ifndef INTERPRETER_VM_H_
#define INTERPRETER_VM_H_

#include <fsi/util/containers.h>

typedef enum ForthVMErr {
    FORTHVM_ERR_OK,
    FORTHVM_ERR_NULL_PTR,
    FORTHVM_ERR_OUT_OF_MEMORY
} ForthVMErr;

#define OFFSET_COMPILED 0x8000000000000000

typedef struct ForthVM {
    String words;
    DArrayOffset offset;
    String compiled;
    String interpreted;
    size_t ip;
    DArrayOffset operation_stack;
    DArrayOffset control_stack;
    DArrayOffset return_stack;
    String memory;
} ForthVM;

ForthVMErr ForthVM_initialize(ForthVM *vm);
void ForthVM_finalize(ForthVM *vm);
ForthVMErr ForthVM_load(ForthVM *vm, char *str);
ForthVMErr ForthVM_execute(ForthVM *vm);

#endif
