#include <stdio.h>

#include <handler.h>

int vm_handler_emit(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    putchar(*(uintptr_t*)vm->data_stack_cur);
    return VM_STATUS_OK;
}
