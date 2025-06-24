#include "handler.h"

int vm_handler_pushdfd(ForthVM *vm) {
    if (vm->data_stack_cur + sizeof(uintptr_t) > vm->data_stack_end) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_OVERFLOW;
    }
    *(uintptr_t*)vm->data_stack_cur =
        (vm->float_stack_cur - vm->float_stack) / sizeof(double);
    vm->data_stack_cur += sizeof(uintptr_t);
    return VM_STATUS_OK;
}
