#include "handler.h"

int vm_handler_stcd(ForthVM *vm) {
    if (vm->data_stack_cur - (sizeof(uintptr_t) << 1) < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t) << 1;
    if (
        *(uint8_t**)(vm->data_stack_cur + sizeof(uintptr_t)) <
        vm->memory ||
        *(uint8_t**)(vm->data_stack_cur + sizeof(uintptr_t)) >=
        *(uint8_t**)vm->memory_cur) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_INVALID_MEMORY_ADDRESS;
    }
    **(uint8_t**)(vm->data_stack_cur + sizeof(uintptr_t)) =
        *(uint8_t*)vm->data_stack_cur;
    return VM_STATUS_OK;
}
