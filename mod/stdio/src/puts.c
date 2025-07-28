#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

int vm_ext_puts(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    puts(*(char**)vm->data_stack_cur);
    return VM_STATUS_OK;
}
