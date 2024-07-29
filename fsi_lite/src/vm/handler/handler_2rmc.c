#include "handler.h"
#include "../vm.h"
#include "../../util/status.h"

int vm_handler_2rmc(void) {
    if (vm_control_stack_cur - (sizeof(uintptr_t) << 1) < vm_control_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    vm_control_stack_cur -= (sizeof(uintptr_t) << 1);
    return VM_STATUS_OK;
}
