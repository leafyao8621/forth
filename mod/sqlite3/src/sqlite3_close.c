#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

#include <sqlite3.h>

int vm_ext_sqlite3_close(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    int ret =
        sqlite3_close(
            *(sqlite3**)(vm->data_stack_cur - sizeof(uintptr_t))
        );
    *(uintptr_t*)vm->data_stack_cur = ret;
    return PARSER_STATUS_OK;
}
