#include "handler.h"
#include "../ext/ext.h"

int vm_handler_callext(ForthVM *vm) {
    vm->ip += sizeof(uintptr_t);
    return (*(Ext*)(vm->ip - sizeof(uintptr_t) + 1))(vm);
}
