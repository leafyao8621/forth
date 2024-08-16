#include "handler.h"
#include "../vm.h"
#include "../../util/status.h"

int vm_handler_3peekc(void) {
    if (vm_control_stack_cur - 3 * sizeof(uintptr_t) < vm_control_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    if (vm_data_stack_cur == vm_data_stack_end) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_OVERFLOW;
    }
    *(uintptr_t*)vm_data_stack_cur =
        *(uintptr_t*)(vm_control_stack_cur - 3 * sizeof(uintptr_t));
    vm_data_stack_cur += sizeof(uintptr_t);
    return VM_STATUS_OK;
}
