#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_allot(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    int ret = DArrayOffset_pop_back(&vm->data_stack);
    if (ret) {
        return FORTHVM_ERR_STACK_UNDERFLOW;
    }
    size_t size = vm->data_stack.data[vm->data_stack.size];
    ret =
        DArrayChar_expand(&vm->memory, size, true);
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    return FORTHVM_ERR_OK;
}
