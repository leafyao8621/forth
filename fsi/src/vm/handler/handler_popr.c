#include "handler.h"

int vm_handler_popr(ForthVM *vm) {
    if (vm->return_stack_cur == vm->return_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    vm->return_stack_cur -= sizeof(uintptr_t);
    if (vm->data_stack_cur == vm->data_stack_end) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_OVERFLOW;
    }
    *(uintptr_t*)vm->data_stack_cur =
        *(uintptr_t*)vm->return_stack_cur;
    vm->data_stack_cur += sizeof(uintptr_t);
    return VM_STATUS_OK;
}
