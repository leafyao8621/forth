#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_pushw(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    int ret = 0;
    if (!(vm->ip & IP_COMPILED)) {
        size_t value =
            vm->offset.data[*(size_t*)(vm->interpreted.data + vm->ip + 1)];
        vm->ip += sizeof(size_t);
        ret = DArrayOffset_push_back(&vm->data_stack, &value);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
    } else {
        size_t value =
            vm->offset.data[
                *(size_t*)(vm->compiled.data + (vm->ip & ~IP_COMPILED) + 1)
            ];
        vm->ip += sizeof(size_t);
        ret = DArrayOffset_push_back(&vm->data_stack, &value);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
    }
    return FORTHVM_ERR_OK;
}
