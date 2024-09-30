#include "handler.h"

int vm_handler_swapd(ForthVM *vm) {
    if (vm->data_stack_cur - (sizeof(uintptr_t) << 1) < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) ^=
        *(uintptr_t*)(vm->data_stack_cur - (sizeof(uintptr_t) << 1));
    *(uintptr_t*)(vm->data_stack_cur - (sizeof(uintptr_t) << 1)) ^=
        *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t));
    *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) ^=
        *(uintptr_t*)(vm->data_stack_cur - (sizeof(uintptr_t) << 1));
    return VM_STATUS_OK;
}
