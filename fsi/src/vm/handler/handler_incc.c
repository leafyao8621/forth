#include "handler.h"

int vm_handler_incc(ForthVM *vm) {
    if (vm->control_stack_cur == vm->control_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    ++(*(uintptr_t*)(vm->control_stack_cur - sizeof(uintptr_t)));
    return VM_STATUS_OK;
}
