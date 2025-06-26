#include "handler.h"

int vm_handler_minf(ForthVM *vm) {
    if (vm->float_stack_cur - (sizeof(double) << 1) < vm->float_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->float_stack_cur -= sizeof(double);
    *(double*)(vm->float_stack_cur - sizeof(double)) =
        *(double*)vm->float_stack_cur <
        *(double*)(vm->float_stack_cur - sizeof(double)) ?
        *(double*)vm->float_stack_cur :
        *(double*)(vm->float_stack_cur - sizeof(double));
    return VM_STATUS_OK;
}
