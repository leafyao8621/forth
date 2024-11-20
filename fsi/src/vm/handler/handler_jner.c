#include "handler.h"

int vm_handler_jner(ForthVM *vm) {
    if (vm->return_stack_cur - (sizeof(uintptr_t) << 1) < vm->return_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    if (
        *(intptr_t*)(vm->return_stack_cur - sizeof(uintptr_t)) !=
        *(intptr_t*)(vm->return_stack_cur - (sizeof(uintptr_t) << 1))) {
        vm->ip = *(uint8_t**)(vm->ip + 1);
    } else {
        vm->ip += sizeof(uintptr_t);
    }
    return VM_STATUS_OK;
}
