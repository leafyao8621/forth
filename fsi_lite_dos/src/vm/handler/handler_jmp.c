#include <stdio.h>

#include "handler.h"
#include "../vm.h"
#include "../../util/status.h"

int vm_handler_jmp(void) {
    vm_ip = *(uint8_t**)(vm_ip + 1);
    return VM_STATUS_OK;
}
