#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_dup(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    if (!vm->data_stack.size) {
        return FORTHVM_ERR_STACK_UNDERFLOW;
    }
    size_t val = vm->data_stack.data[vm->data_stack.size - 1];
    int ret =
        DArrayOffset_push_back(
            &vm->data_stack,
            &val
        );
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    return FORTHVM_ERR_OK;
}
