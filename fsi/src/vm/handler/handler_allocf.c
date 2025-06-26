#include "handler.h"

int vm_handler_allocf(ForthVM *vm) {
    if (*(uint8_t**)vm->memory_cur + sizeof(double) > vm->memory_end) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_INVALID_MEMORY_ALLOCATION;
    }
    *(uint8_t**)vm->memory_cur += sizeof(double);
    return VM_STATUS_OK;
}
