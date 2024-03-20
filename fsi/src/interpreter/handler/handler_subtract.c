#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_subtract(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    if (vm->operation_stack.size < 2) {
        return FORTHVM_ERR_STACK_UNDERFLOW;
    }
    DArrayOffset_pop_back(&vm->operation_stack);
    vm->operation_stack.data[vm->operation_stack.size - 1] -=
        vm->operation_stack.data[vm->operation_stack.size];
    return FORTHVM_ERR_OK;
}
