#include "handler.h"

int vm_handler_popf(ForthVM *vm) {
    if (vm->float_stack_cur == vm->float_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_FLOAT_STACK_UNDERFLOW;
    }
    vm->float_stack_cur -= sizeof(double);
    if (vm->data_stack_cur + sizeof(double) > vm->data_stack_end) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_OVERFLOW;
    }
    *(double*)vm->data_stack_cur =
        *(double*)vm->float_stack_cur;
    vm->data_stack_cur += sizeof(double);
    return VM_STATUS_OK;
}
