#include "handler.h"

int vm_handler_ldd(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    if (
        !(
            (
                *(uint8_t**)(vm->data_stack_cur - sizeof(uintptr_t)) >=
                vm->memory &&
                *(uint8_t**)(vm->data_stack_cur - sizeof(uintptr_t)) <
                *(uint8_t**)vm->memory_cur
            ) ||
            (
                *(uint8_t**)(vm->data_stack_cur - sizeof(uintptr_t)) >=
                vm->literal &&
                *(uint8_t**)(vm->data_stack_cur - sizeof(uintptr_t)) <
                *(uint8_t**)vm->literal_cur
            )
        )) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_INVALID_MEMORY_ADDRESS;
    }
    *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) =
        **(uintptr_t**)(vm->data_stack_cur - sizeof(uintptr_t));
    return VM_STATUS_OK;
}
