#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_drop(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    int ret = DArrayOffset_pop_back(&vm->data_stack);
    if (ret) {
        return FORTHVM_ERR_STACK_UNDERFLOW;
    }
    return FORTHVM_ERR_OK;
}
