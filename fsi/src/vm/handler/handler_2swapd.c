#include "handler.h"

int vm_handler_2swapd(ForthVM *vm) {
    if (vm->data_stack_cur - (sizeof(uintptr_t) << 2) < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) ^=
        *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t) * 3);
    *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t) * 3) ^=
        *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t));
    *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) ^=
        *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t) * 3);
    *(uintptr_t*)(vm->data_stack_cur - (sizeof(uintptr_t) << 1)) ^=
        *(uintptr_t*)(vm->data_stack_cur - (sizeof(uintptr_t) << 2));
    *(uintptr_t*)(vm->data_stack_cur - (sizeof(uintptr_t) << 2)) ^=
        *(uintptr_t*)(vm->data_stack_cur - (sizeof(uintptr_t) << 1));
    *(uintptr_t*)(vm->data_stack_cur - (sizeof(uintptr_t) << 1)) ^=
        *(uintptr_t*)(vm->data_stack_cur - (sizeof(uintptr_t) << 2));
    return VM_STATUS_OK;
}
