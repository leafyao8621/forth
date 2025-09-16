#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

#include <json/document.h>

int vm_ext_JSONDocument_finalize(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    int ret =
        JSONDocument_finalize(
            *(JSONDocument**)(vm->data_stack_cur - sizeof(uintptr_t))
        );
    *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) = ret;
    return PARSER_STATUS_OK;
}
