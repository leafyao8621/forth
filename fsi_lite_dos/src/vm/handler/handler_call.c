#include "handler.h"
#include "../vm.h"
#include "../../util/status.h"

int vm_handler_call(void) {
    if (vm_control_stack_cur + sizeof(uintptr_t) >= vm_control_stack_end) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_OVERFLOW;
    }
    *(uintptr_t*)vm_control_stack_cur = (uintptr_t)(vm_ip + sizeof(uintptr_t));
    vm_control_stack_cur += sizeof(uintptr_t);
    vm_ip = *(uint8_t**)(vm_ip + 1);
    return VM_STATUS_OK;
}
