#include "handler.h"

int vm_handler_2std(ForthVM *vm) {
    if (vm->data_stack_cur - sizeof(uintptr_t) * 3 < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t) * 3;
    if (
        *(uint8_t**)(vm->data_stack_cur + (sizeof(uintptr_t) << 1)) <
        vm->memory ||
        *(uint8_t**)(vm->data_stack_cur + (sizeof(uintptr_t) << 1)) >=
        *(uint8_t**)vm->memory_cur ||
        *(uint8_t**)(vm->data_stack_cur + (sizeof(uintptr_t) << 1)) +
        sizeof(uintptr_t) <
        vm->memory ||
        *(uint8_t**)(vm->data_stack_cur + (sizeof(uintptr_t) << 1)) +
        sizeof(uintptr_t) >=
        *(uint8_t**)vm->memory_cur) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_INVALID_MEMORY_ADDRESS;
    }
    **(uintptr_t**)(vm->data_stack_cur + (sizeof(uintptr_t) << 1)) =
        *(uintptr_t*)(vm->data_stack_cur + sizeof(uintptr_t));
    *(*(uintptr_t**)(vm->data_stack_cur + (sizeof(uintptr_t) << 1)) + 1) =
        *(uintptr_t*)vm->data_stack_cur;
    return VM_STATUS_OK;
}
