#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_multiply(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    if (vm->data_stack.size < 2) {
        return FORTHVM_ERR_STACK_UNDERFLOW;
    }
    DArrayOffset_pop_back(&vm->data_stack);
    vm->data_stack.data[vm->data_stack.size - 1] *=
        vm->data_stack.data[vm->data_stack.size];
    return FORTHVM_ERR_OK;
}
