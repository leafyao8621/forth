#include "handler.h"

int vm_handler_dupf(ForthVM *vm) {
    if (vm->float_stack_cur == vm->float_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_FLOAT_STACK_UNDERFLOW;
    }
    if (vm->float_stack_cur == vm->float_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_FLOAT_STACK_OVERFLOW;
    }
    *(double*)vm->float_stack_cur =
        *(double*)(vm->float_stack_cur - sizeof(double));
    vm->float_stack_cur += sizeof(double);
    return VM_STATUS_OK;
}
