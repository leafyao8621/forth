#include <handler.h>

int vm_handler_ultd(ForthVM *vm) {
    if (vm->data_stack_cur - (sizeof(uintptr_t) << 1) < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) =
        *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) <
        *(uintptr_t*)vm->data_stack_cur ? -1 : 0;
    return VM_STATUS_OK;
}
