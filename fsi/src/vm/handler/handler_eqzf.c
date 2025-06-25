#include "handler.h"

int vm_handler_eqzf(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack_end) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_OVERFLOW;
    }
    if (vm->float_stack_cur == vm->float_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_FLOAT_STACK_UNDERFLOW;
    }
    *(intptr_t*)vm->data_stack_cur =
        *(double*)(vm->float_stack_cur - sizeof(double)) ? -1 : 0;
    vm->data_stack_cur += sizeof(uintptr_t);
    return VM_STATUS_OK;
}
