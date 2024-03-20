#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_return(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    int ret = 0;
    ret = DArrayOffset_pop_back(&vm->return_stack);
    if (ret) {
        return FORTHVM_ERR_RETURN_STACK_EMPTY;
    }
    vm->ip = vm->return_stack.data[vm->return_stack.size] - 1;
    return FORTHVM_ERR_OK;
}
