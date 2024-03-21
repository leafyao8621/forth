#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_return(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    int ret = 0;
    ret = DArrayOffset_pop_back(&vm->call_stack);
    if (ret) {
        return FORTHVM_ERR_RETURN_STACK_EMPTY;
    }
    vm->ip = vm->call_stack.data[vm->call_stack.size] + 8;
    return FORTHVM_ERR_OK;
}
