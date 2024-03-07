#include <stdio.h>

#include <fsi/util/interpreter_handlers.h>

ForthVMErr interpreter_handle_print_int(ForthVM *vm) {
    if (!vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    int ret = DArrayOffset_pop_back(&vm->operation_stack);
    if (ret) {
        return FORTHVM_ERR_STACK_UNDERFLOW;
    }
    size_t value = vm->operation_stack.data[vm->operation_stack.size];
    printf("%ld", (long)value);
    return FORTHVM_ERR_OK;
}
