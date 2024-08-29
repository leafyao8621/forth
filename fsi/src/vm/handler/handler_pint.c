#include <stdio.h>

#include "handler.h"
#include "../vm.h"
#include "../../util/status.h"

int vm_handler_pint(void) {
    if (vm_data_stack_cur == vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm_data_stack_cur -= sizeof(uintptr_t);
    switch (*vm_memory) {
    case 10:
        printf("%ld", *(intptr_t*)vm_data_stack_cur);
        break;
    case 16:
        printf("0x%016lx", *(intptr_t*)vm_data_stack_cur);
        break;
    default:
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_INVALID_BASE;
    }
    return VM_STATUS_OK;
}
