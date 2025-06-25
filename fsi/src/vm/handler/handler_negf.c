#include "handler.h"

int vm_handler_negf(ForthVM *vm) {
    if (vm->float_stack_cur == vm->float_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_FLOAT_STACK_UNDERFLOW;
    }
    *(double*)(vm->float_stack_cur - sizeof(double)) =
        -*(double*)(vm->float_stack_cur - sizeof(double));
    return VM_STATUS_OK;
}
