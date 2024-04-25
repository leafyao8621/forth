#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_st(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    if (vm->data_stack.size < 2) {
        return FORTHVM_ERR_STACK_UNDERFLOW;
    }
    DArrayOffset_pop_back(&vm->data_stack);
    size_t offset = vm->data_stack.data[vm->data_stack.size];
    if (offset >= vm->memory.size) {
        return FORTHVM_ERR_ILLEGAL_ACCESS;
    }
    DArrayOffset_pop_back(&vm->data_stack);
    size_t value = vm->data_stack.data[vm->data_stack.size];
    *(size_t*)(vm->memory.data + offset) = value;
    return FORTHVM_ERR_OK;
}
