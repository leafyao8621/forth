#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

#include <sqlite3.h>

int vm_ext_sqlite3_bind_int(ForthVM *vm) {
    if (vm->data_stack_cur - sizeof(uintptr_t) * 3 < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t) << 1;
    int ret = 0;
    if (sizeof(sqlite3_int64) > sizeof(uintptr_t)) {
        ret =
            sqlite3_bind_int(
                *(sqlite3_stmt**)(vm->data_stack_cur - sizeof(uintptr_t)),
                *(int*)vm->data_stack_cur,
                *(int*)(vm->data_stack_cur + sizeof(uintptr_t))
            );
    } else {
        ret =
            sqlite3_bind_int64(
                *(sqlite3_stmt**)(vm->data_stack_cur - sizeof(uintptr_t)),
                *(int*)vm->data_stack_cur,
                *(sqlite3_int64*)(vm->data_stack_cur + sizeof(uintptr_t))
            );
    }
    *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) = ret;
    return PARSER_STATUS_OK;
}

int vm_ext_sqlite3_bind_float(ForthVM *vm) {
    if (vm->data_stack_cur - (sizeof(uintptr_t) << 1) < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    if (vm->float_stack_cur == vm->float_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_FLOAT_STACK_UNDERFLOW;
    }
    vm->float_stack_cur -= sizeof(uintptr_t);
    int ret =
        sqlite3_bind_double(
            *(sqlite3_stmt**)(vm->data_stack_cur - sizeof(uintptr_t)),
            *(int*)vm->data_stack_cur,
            *(double*)vm->float_stack_cur
        );
    *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) = ret;
    return PARSER_STATUS_OK;
}

int vm_ext_sqlite3_bind_null(ForthVM *vm) {
    if (vm->data_stack_cur - (sizeof(uintptr_t) << 1) < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    int ret =
        sqlite3_bind_null(
            *(sqlite3_stmt**)(vm->data_stack_cur - sizeof(uintptr_t)),
            *(int*)vm->data_stack_cur
        );
    *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) = ret;
    return PARSER_STATUS_OK;
}

int vm_ext_sqlite3_bind_text(ForthVM *vm) {
    if (vm->data_stack_cur - sizeof(uintptr_t) * 3 < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t) << 1;
    int ret =
        sqlite3_bind_text(
            *(sqlite3_stmt**)(vm->data_stack_cur - sizeof(uintptr_t)),
            *(int*)vm->data_stack_cur,
            *(const char**)(vm->data_stack_cur + sizeof(uintptr_t)),
            -1,
            0
        );
    *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) = ret;
    return PARSER_STATUS_OK;
}
