#include "handler.h"

int vm_handler_addr(ForthVM *vm) {
    if (vm->return_stack_cur == vm->return_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    *(intptr_t*)(vm->return_stack_cur - sizeof(uintptr_t)) +=
        *(intptr_t*)vm->data_stack_cur;
    return VM_STATUS_OK;
}
