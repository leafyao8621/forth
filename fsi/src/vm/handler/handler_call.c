#include <handler.h>

int vm_handler_call(ForthVM *vm) {
    if (vm->control_stack_cur + sizeof(uintptr_t) >= vm->control_stack_end) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_OVERFLOW;
    }
    *(uintptr_t*)vm->control_stack_cur = (uintptr_t)(vm->ip + sizeof(uintptr_t));
    vm->control_stack_cur += sizeof(uintptr_t);
    vm->ip = *(uint8_t**)(vm->ip + 1);
    return VM_STATUS_OK;
}
