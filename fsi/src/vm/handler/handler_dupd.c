#include "handler.h"

int vm_handler_dupd(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    if (vm->data_stack_cur + sizeof(uintptr_t) > vm->data_stack_end) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_OVERFLOW;
    }
    *(uintptr_t*)vm->data_stack_cur =
        *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t));
    vm->data_stack_cur += sizeof(uintptr_t);
    return VM_STATUS_OK;
}
