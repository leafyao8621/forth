#include "handler.h"

int vm_handler_divf(ForthVM *vm) {
    if (vm->float_stack_cur - (sizeof(double) << 1) < vm->float_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_FLOAT_STACK_UNDERFLOW;
    }
    vm->float_stack_cur -= sizeof(double);
    *(double*)(vm->float_stack_cur - sizeof(double)) /=
        *(double*)vm->float_stack_cur;
    return VM_STATUS_OK;
}
