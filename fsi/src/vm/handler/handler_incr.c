#include "handler.h"

int vm_handler_incr(ForthVM *vm) {
    if (vm->return_stack_cur == vm->return_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    ++(*(uintptr_t*)(vm->return_stack_cur - sizeof(uintptr_t)));
    return VM_STATUS_OK;
}
