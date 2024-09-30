#include <stdio.h>

#include <handler.h>

int vm_handler_pint(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    switch (*vm->memory) {
    case 10:
        printf("%ld", *(intptr_t*)vm->data_stack_cur);
        break;
    case 16:
        printf("0x%016lx", *(intptr_t*)vm->data_stack_cur);
        break;
    default:
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_INVALID_BASE;
    }
    return VM_STATUS_OK;
}
