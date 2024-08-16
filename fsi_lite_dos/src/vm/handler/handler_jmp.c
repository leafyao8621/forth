#include "handler.h"
#include "src\vm\vm.h"
#include "src\util\status.h"

int vm_handler_jmp(void) {
    vm_ip = *(uint8_t**)(vm_ip + 1);
    return VM_STATUS_OK;
}
