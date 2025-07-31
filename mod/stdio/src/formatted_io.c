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

int vm_ext_printf_dot_f(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    if (vm->float_stack_cur == vm->float_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_FLOAT_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    vm->float_stack_cur -= sizeof(double);
    printf(
        *(char**)vm->data_stack_cur,
        *(double*)vm->float_stack_cur
    );
    return VM_STATUS_OK;
}

int vm_ext_fprintf_dot_d(ForthVM *vm) {
    if (vm->data_stack_cur - sizeof(uintptr_t) * 3 < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t) * 3;
    fprintf(
        *(FILE**)vm->data_stack_cur,
        *(char**)(vm->data_stack_cur + sizeof(uintptr_t)),
        *(uintptr_t*)(vm->data_stack_cur + (sizeof(uintptr_t) << 1))
    );
    return VM_STATUS_OK;
}

int vm_ext_fprintf_dot_f(ForthVM *vm) {
    if (vm->data_stack_cur - (sizeof(uintptr_t) << 1) < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    if (vm->float_stack_cur == vm->float_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_FLOAT_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t) << 1;
    vm->float_stack_cur -= sizeof(double);
    fprintf(
        *(FILE**)vm->data_stack_cur,
        *(char**)(vm->data_stack_cur + sizeof(uintptr_t)),
        *(double*)vm->float_stack_cur
    );
    return VM_STATUS_OK;
}
