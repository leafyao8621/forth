#include "handler.h"
#include "../vm.h"
#include "../../util/status.h"

int vm_handler_ret(void) {
    if (vm_control_stack_cur == vm_control_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    vm_control_stack_cur -= sizeof(uintptr_t);
    vm_ip = *(uint8_t**)vm_control_stack_cur;
    return VM_STATUS_OK;
}
