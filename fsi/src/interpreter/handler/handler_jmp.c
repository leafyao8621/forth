#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_jmp(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    vm->ip =
        (*(size_t*)(vm->compiled.data + (vm->ip & ~IP_COMPILED) + 1)) |
        IP_COMPILED;
    return FORTHVM_ERR_OK;
}
