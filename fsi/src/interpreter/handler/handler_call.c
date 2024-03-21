#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_call(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    int ret = 0;
    size_t value =
        !(vm->ip & IP_COMPILED) ?
        *(size_t*)(vm->interpreted.data + vm->ip + 1) :
        *(size_t*)(vm->compiled.data + (vm->ip & ~IP_COMPILED) + 1);
    ret = DArrayOffset_push_back(&vm->return_stack, &vm->ip);
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    vm->ip = (value | IP_COMPILED) - 1;
    return FORTHVM_ERR_OK;
}
