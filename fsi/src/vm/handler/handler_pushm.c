#include "handler.h"

int vm_handler_pushm(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    if (
        *(uint8_t**)vm->memory_cur + sizeof(uintptr_t) >
        vm->memory_end) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_INVALID_MEMORY_ALLOCATION;
    }
    **(uintptr_t**)vm->memory_cur = *(intptr_t*)vm->data_stack_cur;
    *(uint8_t**)vm->memory_cur += sizeof(uintptr_t);
    return VM_STATUS_OK;
}
