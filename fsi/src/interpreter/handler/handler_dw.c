#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_dw(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    size_t value = *(size_t*)(vm->interpreted.data + vm->ip + 1);
    vm->ip += sizeof(size_t);
    vm->offset.data[value] = vm->memory.size;
    return FORTHVM_ERR_OK;
}
