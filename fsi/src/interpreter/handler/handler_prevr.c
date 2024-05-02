#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_prevr(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    if (vm->return_stack.size < 3) {
        return FORTHVM_ERR_STACK_UNDERFLOW;
    }
    int ret =
        DArrayOffset_push_back(
            &vm->data_stack,
            vm->return_stack.data + vm->return_stack.size - 3
        );
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    return FORTHVM_ERR_OK;
}
