#include "handler.h"

int vm_handler_pickd(ForthVM *vm) {
    if (vm->data_stack_cur - sizeof(uintptr_t) < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    if (
        vm->data_stack_cur - sizeof(uintptr_t) *
        (*(intptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) + 2) <
        vm->data_stack) {
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    *(intptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) =
        *(intptr_t*)
            (
                vm->data_stack_cur - sizeof(uintptr_t) *
                (*(intptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) + 2)
            );
    return VM_STATUS_OK;
}
