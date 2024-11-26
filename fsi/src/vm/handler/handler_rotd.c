#include "handler.h"

int vm_handler_rotd(ForthVM *vm) {
    if (vm->data_stack_cur - sizeof(uintptr_t) * 3 < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    intptr_t temp = *(intptr_t*)(vm->data_stack_cur - sizeof(uintptr_t) * 3);
    *(intptr_t*)(vm->data_stack_cur - sizeof(uintptr_t) * 3) =
        *(intptr_t*)(vm->data_stack_cur - (sizeof(uintptr_t) << 1));
    *(intptr_t*)(vm->data_stack_cur - (sizeof(uintptr_t) << 1)) =
        *(intptr_t*)(vm->data_stack_cur - sizeof(uintptr_t));
    *(intptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) = temp;
    return VM_STATUS_OK;
}
