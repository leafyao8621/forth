#include "handler.h"

int vm_handler_rotf(ForthVM *vm) {
    if (vm->float_stack_cur - sizeof(uintptr_t) * 3 < vm->float_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_FLOAT_STACK_UNDERFLOW;
    }
    double temp = *(double*)(vm->float_stack_cur - sizeof(double) * 3);
    *(double*)(vm->float_stack_cur - sizeof(double) * 3) =
        *(double*)(vm->float_stack_cur - (sizeof(double) << 1));
    *(double*)(vm->float_stack_cur - (sizeof(double) << 1)) =
        *(double*)(vm->float_stack_cur - sizeof(double));
    *(double*)(vm->float_stack_cur - sizeof(double)) = temp;
    return VM_STATUS_OK;
}
