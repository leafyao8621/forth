#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

#include "mt19937.h"

int vm_ext_mt19937_initialize(ForthVM *vm) {
    if (vm->data_stack_cur - (sizeof(uintptr_t) << 1) < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t) << 1;
    mt19937_initialize(
        *(struct MT19937**)vm->data_stack_cur,
        *(unsigned int*)(vm->data_stack_cur + sizeof(uintptr_t))
    );
    return PARSER_STATUS_OK;
}

int vm_ext_mt19937_gen(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) =
        mt19937_gen(
            *(struct MT19937**)(vm->data_stack_cur - sizeof(uintptr_t))
        );
    return PARSER_STATUS_OK;
}
