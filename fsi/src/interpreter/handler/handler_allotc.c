#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_allotc(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    size_t value = 0;
    int ret =
        DArrayChar_push_back_batch(&vm->memory, (char*)&value, sizeof(size_t));
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    return FORTHVM_ERR_OK;
}
