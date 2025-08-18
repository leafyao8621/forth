#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

#include <sqlite3.h>

int vm_ext_sqlite3_column_int(ForthVM *vm) {
    if (vm->data_stack_cur - (sizeof(uintptr_t) << 1) < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    uintptr_t ret = 0;
    if (sizeof(sqlite3_int64) > sizeof(uintptr_t)) {
        ret =
            sqlite3_column_int(
                *(sqlite3_stmt**)(vm->data_stack_cur - sizeof(uintptr_t)),
                *(int*)vm->data_stack_cur
            );
    } else {
        ret =
            sqlite3_column_int64(
                *(sqlite3_stmt**)(vm->data_stack_cur - sizeof(uintptr_t)),
                *(int*)vm->data_stack_cur
            );
    }
    *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) = ret;
    return PARSER_STATUS_OK;
}

int vm_ext_sqlite3_column_float(ForthVM *vm) {
    if (vm->data_stack_cur - (sizeof(uintptr_t) << 1) < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= (sizeof(uintptr_t) << 1);
    if (vm->float_stack_cur == vm->float_stack_end) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_FLOAT_STACK_OVERFLOW;
    }
    double ret =
        sqlite3_column_double(
            *(sqlite3_stmt**)vm->data_stack_cur,
            *(int*)(vm->data_stack_cur + sizeof(uintptr_t))
        );
    *(double*)vm->float_stack_cur = ret;
    vm->float_stack_cur += sizeof(double);
    return PARSER_STATUS_OK;
}

int vm_ext_sqlite3_column_text(ForthVM *vm) {
    if (vm->data_stack_cur - (sizeof(uintptr_t) << 1) < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    const void *ret =
        sqlite3_column_text(
            *(sqlite3_stmt**)(vm->data_stack_cur - sizeof(uintptr_t)),
            *(int*)vm->data_stack_cur
        );
    *(const void**)(vm->data_stack_cur - sizeof(uintptr_t)) = ret;
    return PARSER_STATUS_OK;
}
