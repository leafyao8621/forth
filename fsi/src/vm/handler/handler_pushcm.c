#include "handler.h"

int vm_handler_pushcm(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    if (*(uint8_t**)vm->memory_cur == vm->memory_end) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_INVALID_MEMORY_ALLOCATION;
    }
    **(uint8_t**)vm->memory_cur = *(uint8_t*)vm->data_stack_cur;
    ++(*(uint8_t**)vm->memory_cur);
    return VM_STATUS_OK;
}
