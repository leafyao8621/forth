#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_incr(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    if (!vm->return_stack.size) {
        return FORTHVM_ERR_STACK_UNDERFLOW;
    }
    ++vm->return_stack.data[vm->return_stack.size - 1];
    return FORTHVM_ERR_OK;
}
