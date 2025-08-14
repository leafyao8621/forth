#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

#include <sqlite3.h>

int vm_ext_sqlite3_open(ForthVM *vm) {
    if (vm->data_stack_cur - (sizeof(uintptr_t) << 1) < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    int ret =
        sqlite3_open(
            *(const void**)(vm->data_stack_cur - sizeof(uintptr_t)),
            *(sqlite3***)vm->data_stack_cur
        );
    *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) = ret;
    return PARSER_STATUS_OK;
}
