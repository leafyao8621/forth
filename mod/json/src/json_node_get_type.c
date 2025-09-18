#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

#include <json/document.h>

int vm_ext_json_node_get_type(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) =
        (*(JSONNode**)(vm->data_stack_cur - sizeof(uintptr_t)))->type;
    return PARSER_STATUS_OK;
}
