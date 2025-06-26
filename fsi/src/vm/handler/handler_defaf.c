#include "handler.h"

int vm_handler_defaf(ForthVM *vm) {
    **(uint8_t***)(vm->ip + 1) = *(uint8_t**)vm->memory_cur;
    vm->ip += sizeof(uintptr_t);
    if (*(uint8_t**)vm->memory_cur + sizeof(double) > vm->memory_end) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_INVALID_MEMORY_ALLOCATION;
    }
    *(uint8_t**)vm->memory_cur += sizeof(double);
    return VM_STATUS_OK;
}
