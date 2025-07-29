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

int vm_ext_putchar(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    putchar(*(uintptr_t*)vm->data_stack_cur);
    return VM_STATUS_OK;
}

int vm_ext_fputs(ForthVM *vm) {
    if (vm->data_stack_cur - (sizeof(uintptr_t) << 1) < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t) << 1;
    fputs(
        *(char**)vm->data_stack_cur,
        *(FILE**)(vm->data_stack_cur + sizeof(uintptr_t))
    );
    return VM_STATUS_OK;
}

int vm_ext_fputc(ForthVM *vm) {
    if (vm->data_stack_cur - (sizeof(uintptr_t) << 1) < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t) << 1;
    fputc(
        *(uintptr_t*)vm->data_stack_cur,
        *(FILE**)(vm->data_stack_cur + sizeof(uintptr_t))
    );
    return VM_STATUS_OK;
}
