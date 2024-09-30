#include <handler.h>

int vm_handler_ret(ForthVM *vm) {
    if (vm->control_stack_cur == vm->control_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_CONTROL_STACK_UNDERFLOW;
    }
    vm->control_stack_cur -= sizeof(uintptr_t);
    if (
        !(
            (
                *(uint8_t**)vm->control_stack_cur >= vm->compiled &&
                *(uint8_t**)vm->control_stack_cur < vm->compiled_end
            ) ||
            (
                *(uint8_t**)vm->control_stack_cur >= vm->interpreted &&
                *(uint8_t**)vm->control_stack_cur < vm->interpreted_end
            )
        )) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_INVALID_RETURN_ADDRESS;
    }
    vm->ip = *(uint8_t**)vm->control_stack_cur;
    return VM_STATUS_OK;
}
