#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

int vm_ext_printf_dot_d(ForthVM *vm) {
    if (vm->data_stack_cur - (sizeof(uintptr_t) << 1) < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t) << 1;
    printf(
        *(char**)vm->data_stack_cur,
        *(uintptr_t*)(vm->data_stack_cur + sizeof(uintptr_t))
    );
    return VM_STATUS_OK;
}
