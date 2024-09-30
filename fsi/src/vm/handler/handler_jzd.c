#include "handler.h"

int vm_handler_jzd(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    if (!*(intptr_t*)vm->data_stack_cur) {
        vm->ip = *(uint8_t**)(vm->ip + 1);
    } else {
        vm->ip += sizeof(uintptr_t);
    }
    return VM_STATUS_OK;
}
