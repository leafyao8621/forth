#include "handler.h"
#include "../vm.h"
#include "../../util/status.h"

int vm_handler_xord(void) {
    if (vm_data_stack_cur - (sizeof(uintptr_t) << 1) < vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) ^=
        *(intptr_t*)vm_data_stack_cur;
    return VM_STATUS_OK;
}
