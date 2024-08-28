#include "handler.h"
#include "../vm.h"
#include "../../util/status.h"

int vm_handler_ltzd(void) {
    if (vm_data_stack_cur == vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) =
        *(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)) < 0 ? -1 : 0;
    return VM_STATUS_OK;
}
