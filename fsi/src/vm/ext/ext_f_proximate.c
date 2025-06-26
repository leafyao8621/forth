#include <math.h>

#include "ext.h"

int vm_ext_f_proximate(ForthVM *vm) {
    if (vm->float_stack_cur - sizeof(double) * 3 < vm->float_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    if (vm->data_stack_cur == vm->data_stack_end) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_OVERFLOW;
    }
    vm->float_stack_cur -= sizeof(double) * 3;
    if (*(double*)(vm->float_stack_cur + (sizeof(double) << 1)) > 0) {
        *(intptr_t*)vm->data_stack_cur =
            fabs(
                *(double*)vm->float_stack_cur -
                *(double*)(vm->float_stack_cur + sizeof(double))
             ) <
            *(double*)(vm->float_stack_cur + (sizeof(double) << 1)) ?
            -1 : 0;
    } else if (*(double*)(vm->float_stack_cur + (sizeof(double) << 1)) == 0) {
        *(intptr_t*)vm->data_stack_cur =
            *(uint64_t*)vm->float_stack_cur ==
            *(uint64_t*)(vm->float_stack_cur + sizeof(double)) ?
            -1 : 0;
    } else {
        *(intptr_t*)vm->data_stack_cur =
            fabs(
                *(double*)vm->float_stack_cur -
                *(double*)(vm->float_stack_cur + sizeof(double))
             ) <
            fabs(*(double*)(vm->float_stack_cur + (sizeof(double) << 1))) *
            (
                fabs(*(double*)vm->float_stack_cur) +
                fabs(*(double*)(vm->float_stack_cur + sizeof(double)))
            ) ?
            -1 : 0;
    }
    vm->data_stack_cur += sizeof(uintptr_t);
    return VM_STATUS_OK;
}
