#include "handler.h"
#include "../vm.h"
#include "../../util/status.h"

int vm_handler_std(void) {
    if (vm_data_stack_cur - (sizeof(uintptr_t) << 1) < vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t) << 1;
    if (
        *(uint8_t**)(vm_data_stack_cur + sizeof(uintptr_t)) <
        vm_memory ||
        *(uint8_t**)(vm_data_stack_cur + sizeof(uintptr_t)) >
        *(uint8_t**)vm_memory_cur) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_INVALID_MEMORY_ADDRESS;
    }
    **(uintptr_t**)(vm_data_stack_cur + sizeof(uintptr_t)) =
        *(uintptr_t*)vm_data_stack_cur;
    return VM_STATUS_OK;
}
