#include "handler.h"

int vm_handler_alloc(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    if (
        *(uint8_t**)vm->memory_cur + *(intptr_t*)(vm->data_stack_cur) >
        vm->memory_end ||
        *(uint8_t**)vm->memory_cur + *(intptr_t*)(vm->data_stack_cur) <
        vm->memory) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_INVALID_MEMORY_ALLOCATION;
    }
    *(uint8_t**)vm->memory_cur += *(intptr_t*)(vm->data_stack_cur);
    return VM_STATUS_OK;
}
