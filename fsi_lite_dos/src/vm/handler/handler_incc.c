#include "handler.h"
#include "src\vm\vm.h"
#include "src\util\status.h"

int vm_handler_incc(void) {
    if (vm_control_stack_cur == vm_control_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    ++(*(uintptr_t*)(vm_control_stack_cur - sizeof(uintptr_t)));
    return VM_STATUS_OK;
}
