#include "handler.h"
#include "src\vm\vm.h"
#include "src\util\status.h"

int vm_handler_decd(void) {
    if (vm_data_stack_cur == vm_data_stack) {
        vm_state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    --(*(intptr_t*)(vm_data_stack_cur - sizeof(uintptr_t)));
    return VM_STATUS_OK;
}
