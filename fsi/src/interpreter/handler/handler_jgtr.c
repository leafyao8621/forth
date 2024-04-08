#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_jgtr(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    if (vm->return_stack.size < 2) {
        return FORTHVM_ERR_STACK_UNDERFLOW;
    }
    if (
        vm->return_stack.data[vm->return_stack.size - 2] <=
        vm->return_stack.data[vm->return_stack.size - 1]) {
        vm->ip += 8;
        return FORTHVM_ERR_OK;
    }
    vm->ip =
        (*(size_t*)(vm->compiled.data + (vm->ip & ~IP_COMPILED) + 1)) |
        IP_COMPILED;
    return FORTHVM_ERR_OK;
}
