#include "handler.h"

int vm_handler_2rmr(ForthVM *vm) {
    if (vm->return_stack_cur - (sizeof(uintptr_t) << 1) < vm->return_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    vm->return_stack_cur -= (sizeof(uintptr_t) << 1);
    return VM_STATUS_OK;
}
