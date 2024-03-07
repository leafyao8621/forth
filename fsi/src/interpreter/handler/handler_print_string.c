#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_print_string(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    if (!(vm->ip & IP_COMPILED)) {
        printf(
            "%s",
            vm->literal.data +
            *(size_t*)(vm->interpreted.data + vm->ip + 1));
        vm->ip += sizeof(size_t);
    }
    return FORTHVM_ERR_OK;
}
