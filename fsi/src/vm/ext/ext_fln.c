#include <math.h>

#include "ext.h"

int vm_ext_fln(ForthVM *vm) {
    if (vm->float_stack_cur == vm->float_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    *(double*)(vm->float_stack_cur - sizeof(double)) =
        log(*(double*)(vm->float_stack_cur - sizeof(double)));
    return VM_STATUS_OK;
}
