#include <stdio.h>

#include "handler.h"

int vm_handler_pfloat(ForthVM *vm) {
    if (vm->float_stack_cur == vm->float_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_FLOAT_STACK_UNDERFLOW;
    }
    vm->float_stack_cur -= sizeof(uintptr_t);
    switch (*vm->memory) {
    case 10:
        printf("%lf", *(double*)vm->float_stack_cur);
        break;
    default:
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_INVALID_BASE;
    }
    return VM_STATUS_OK;
}
