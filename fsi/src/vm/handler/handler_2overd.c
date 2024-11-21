#include "handler.h"

int vm_handler_2overd(ForthVM *vm) {
    if (vm->data_stack_cur - (sizeof(uintptr_t) << 2) < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    if (vm->data_stack_cur + (sizeof(uintptr_t) << 1) >= vm->data_stack_end) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_OVERFLOW;
    }
    *(intptr_t*)vm->data_stack_cur =
        *(intptr_t*)(vm->data_stack_cur - (sizeof(uintptr_t) << 2));
    *(intptr_t*)(vm->data_stack_cur + sizeof(uintptr_t)) =
        *(intptr_t*)(vm->data_stack_cur - sizeof(uintptr_t) * 3);
    vm->data_stack_cur += sizeof(uintptr_t) << 1;
    return VM_STATUS_OK;
}
