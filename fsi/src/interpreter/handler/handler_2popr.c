#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_2popr(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    if (vm->return_stack.size < 2) {
        return FORTHVM_ERR_STACK_UNDERFLOW;
    }
    DArrayOffset_pop_back(&vm->return_stack);
    DArrayOffset_pop_back(&vm->return_stack);
    return FORTHVM_ERR_OK;
}
