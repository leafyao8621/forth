#include "handler.h"
#include "../vm.h"
#include "../../util/status.h"

int vm_handler_def(void) {
    **(uint8_t***)(vm_ip + 1) = *(uint8_t**)vm_memory_cur;
    vm_ip += sizeof(uintptr_t);
    return VM_STATUS_OK;
}
