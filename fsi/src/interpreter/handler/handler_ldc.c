#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_ldc(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    if (!vm->data_stack.size) {
        return FORTHVM_ERR_STACK_UNDERFLOW;
    }
    size_t offset = vm->data_stack.data[vm->data_stack.size - 1];
    if (offset >= vm->memory.size) {
        return FORTHVM_ERR_ILLEGAL_ACCESS;
    }
    vm->data_stack.data[vm->data_stack.size - 1] =
        (uint8_t)vm->memory.data[offset];
    return FORTHVM_ERR_OK;
}
