#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_jzd(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    int ret = DArrayOffset_pop_back(&vm->data_stack);
    if (ret) {
        return FORTHVM_ERR_STACK_UNDERFLOW;
    }
    if (vm->data_stack.data[vm->data_stack.size]) {
        vm->ip += 8;
        return FORTHVM_ERR_OK;
    }
    vm->ip =
        (*(size_t*)(vm->compiled.data + (vm->ip & ~IP_COMPILED) + 1)) |
        IP_COMPILED;
    return FORTHVM_ERR_OK;
}
