#include "handler.h"
#include "src\vm\vm.h"
#include "src\util\status.h"

int vm_handler_jnec(void) {
    if (vm_control_stack_cur - (sizeof(uintptr_t) << 1) < vm_control_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    if (
        *(intptr_t*)(vm_control_stack_cur - sizeof(uintptr_t)) !=
        *(intptr_t*)(vm_control_stack_cur - (sizeof(uintptr_t) << 1))) {
        vm_ip = *(uint8_t**)(vm_ip + 1);
    } else {
        vm_ip += sizeof(uintptr_t);
    }
    return VM_STATUS_OK;
}
