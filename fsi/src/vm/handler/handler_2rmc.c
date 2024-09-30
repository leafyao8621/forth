#include "handler.h"

int vm_handler_2rmc(ForthVM *vm) {
    if (vm->control_stack_cur - (sizeof(uintptr_t) << 1) < vm->control_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    vm->control_stack_cur -= (sizeof(uintptr_t) << 1);
    return VM_STATUS_OK;
}
