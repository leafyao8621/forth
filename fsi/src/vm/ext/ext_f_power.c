#include <math.h>

#include "ext.h"

int vm_ext_f_power(ForthVM *vm) {
    if (vm->float_stack_cur - (sizeof(double) << 1) < vm->float_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_FLOAT_STACK_UNDERFLOW;
    }
    vm->float_stack_cur -= sizeof(double);
    *(double*)(vm->float_stack_cur - sizeof(double)) =
        pow(
            *(double*)(vm->float_stack_cur - sizeof(double)),
            *(double*)vm->float_stack_cur
        );
    return VM_STATUS_OK;
}
