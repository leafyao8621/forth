#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

int vm_ext_fopen(ForthVM *vm) {
    if (vm->data_stack_cur - (sizeof(uintptr_t) << 1) < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    *(FILE**)(vm->data_stack_cur - sizeof(uintptr_t)) =
        fopen(
            *(char**)(vm->data_stack_cur - sizeof(uintptr_t)),
            *(char**)vm->data_stack_cur
        );
    return VM_STATUS_OK;
}

int vm_ext_fclose(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    fclose(*(FILE**)vm->data_stack_cur);
    return VM_STATUS_OK;
}
