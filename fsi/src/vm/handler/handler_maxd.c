#include "handler.h"

int vm_handler_maxd(ForthVM *vm) {
    if (vm->data_stack_cur - (sizeof(uintptr_t) << 1) < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    *(intptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) =
        *(intptr_t*)vm->data_stack_cur >
        *(intptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) ?
        *(intptr_t*)vm->data_stack_cur :
        *(intptr_t*)(vm->data_stack_cur - sizeof(uintptr_t));
    return VM_STATUS_OK;
}
