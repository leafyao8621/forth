#include "handler.h"

int vm_handler_jmp(ForthVM *vm) {
    vm->ip = *(uint8_t**)(vm->ip + 1);
    return VM_STATUS_OK;
}
