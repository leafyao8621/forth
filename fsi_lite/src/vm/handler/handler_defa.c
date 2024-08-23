#include "handler.h"
#include "../vm.h"
#include "../../util/status.h"

int vm_handler_defa(void) {
    **(uint8_t***)(vm_ip + 1) = *(uint8_t**)vm_memory_cur;
    vm_ip += sizeof(uintptr_t);
    if (*(uint8_t**)vm_memory_cur + sizeof(uintptr_t) > vm_memory_end) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_INVALID_MEMORY_ALLOCATION;
    }
    return VM_STATUS_OK;
}
