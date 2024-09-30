#include <handler.h>

int vm_handler_def(ForthVM *vm) {
    **(uint8_t***)(vm->ip + 1) = *(uint8_t**)vm->memory_cur;
    vm->ip += sizeof(uintptr_t);
    return VM_STATUS_OK;
}
