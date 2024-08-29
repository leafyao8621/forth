#include "handler.h"
#include "../vm.h"
#include "../../util/status.h"

int vm_handler_2pushc(void) {
    if (vm_data_stack_cur - (sizeof(uintptr_t) << 1) < vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= (sizeof(uintptr_t) << 1);
    if (
        vm_control_stack_cur + (sizeof(uintptr_t) << 1) >
        vm_control_stack_end) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_OVERFLOW;
    }
    *(uintptr_t*)vm_control_stack_cur = *(uintptr_t*)(vm_data_stack_cur);
    vm_control_stack_cur += sizeof(uintptr_t);
    *(uintptr_t*)vm_control_stack_cur =
        *(uintptr_t*)(vm_data_stack_cur + sizeof(uintptr_t));
    vm_control_stack_cur += sizeof(uintptr_t);
    return VM_STATUS_OK;
}
