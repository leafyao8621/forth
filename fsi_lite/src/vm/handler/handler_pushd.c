#include "handler.h"
#include "../vm.h"
#include "../../util/status.h"

int vm_handler_pushd(void) {
    if (vm_data_stack_cur + sizeof(uintptr_t) > vm_data_stack_end) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_OVERFLOW;
    }
    *(uintptr_t*)vm_data_stack_cur = *(uintptr_t*)(vm_ip + 1);
    vm_data_stack_cur += sizeof(uintptr_t);
    vm_ip += sizeof(uintptr_t);
    return VM_STATUS_OK;
}
