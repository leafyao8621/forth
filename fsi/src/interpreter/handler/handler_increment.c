#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_increment(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    if (!vm->data_stack.size) {
        return FORTHVM_ERR_STACK_UNDERFLOW;
    }
    ++vm->data_stack.data[vm->data_stack.size - 1];
    return FORTHVM_ERR_OK;
}
