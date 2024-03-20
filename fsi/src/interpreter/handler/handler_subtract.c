#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_subtract(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    int ret = DArrayOffset_pop_back(&vm->operation_stack);
    if (ret) {
        return FORTHVM_ERR_STACK_UNDERFLOW;
    }
    size_t value2 = vm->operation_stack.data[vm->operation_stack.size];
    ret = DArrayOffset_pop_back(&vm->operation_stack);
    if (ret) {
        return FORTHVM_ERR_STACK_UNDERFLOW;
    }
    size_t value1 = vm->operation_stack.data[vm->operation_stack.size];
    size_t value = value1 - value2;
    ret = DArrayOffset_push_back(&vm->operation_stack, &value);
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    return FORTHVM_ERR_OK;
}
