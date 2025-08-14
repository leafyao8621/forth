#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

#include <sqlite3.h>

int vm_ext_sqlite3_prepare(ForthVM *vm) {
    if (vm->data_stack_cur - sizeof(uintptr_t) * 3 < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t) << 1;
    int ret =
        sqlite3_prepare_v2(
            *(sqlite3**)(vm->data_stack_cur - sizeof(uintptr_t)),
            *(const char**)vm->data_stack_cur,
            -1,
            *(sqlite3_stmt***)(vm->data_stack_cur + sizeof(uintptr_t)),
            0
        );
    *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) = ret;
    return PARSER_STATUS_OK;
}
