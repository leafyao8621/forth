#include "handler.h"

int vm_handler_overf(ForthVM *vm) {
    if (vm->float_stack_cur - (sizeof(double) << 1) < vm->float_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_FLOAT_STACK_UNDERFLOW;
    }
    if (vm->float_stack_cur + (sizeof(double)) >= vm->float_stack_end) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_FLOAT_STACK_OVERFLOW;
    }
    *(double*)vm->float_stack_cur =
        *(double*)(vm->float_stack_cur - (sizeof(double) << 1));
    vm->float_stack_cur += sizeof(double);
    return VM_STATUS_OK;
}
