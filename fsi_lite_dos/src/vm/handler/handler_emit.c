#include <stdio.h>

#include "handler.h"
#include "src\vm\vm.h"
#include "src\util\status.h"

int vm_handler_emit(void) {
    if (vm_data_stack_cur == vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    putchar(*(uintptr_t*)vm_data_stack_cur);
    return VM_STATUS_OK;
}
