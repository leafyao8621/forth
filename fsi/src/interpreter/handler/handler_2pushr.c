#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_2pushr(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    if (vm->data_stack.size < 2) {
        return FORTHVM_ERR_STACK_UNDERFLOW;
    }
    DArrayOffset_pop_back(&vm->data_stack);
    DArrayOffset_pop_back(&vm->data_stack);
    int ret =
        DArrayOffset_push_back_batch(
            &vm->return_stack,
            vm->data_stack.data + vm->data_stack.size,
            2
        );
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    return FORTHVM_ERR_OK;
}
