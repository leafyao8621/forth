#include "handler.h"

int vm_handler_stf(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    if (
        *(uint8_t**)vm->data_stack_cur <
        vm->memory ||
        *(uint8_t**)vm->data_stack_cur >=
        *(uint8_t**)vm->memory_cur) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_INVALID_MEMORY_ADDRESS;
    }
    if (vm->float_stack_cur == vm->float_stack_end) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_FLOAT_STACK_OVERFLOW;
    }
    vm->float_stack_cur -= sizeof(uintptr_t);
    **(uintptr_t**)vm->data_stack_cur =
        *(uintptr_t*)vm->float_stack_cur;
    return VM_STATUS_OK;
}
