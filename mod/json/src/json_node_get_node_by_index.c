#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

#include <json/document.h>

int vm_ext_json_node_get_node_by_index(ForthVM *vm) {
    if (vm->data_stack_cur - (sizeof(uintptr_t) << 1) < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    if (
        (*(JSONNodePtr*)(vm->data_stack_cur - sizeof(uintptr_t)))->type !=
        ARRAY) {
        *(JSONNodePtr*)(vm->data_stack_cur - sizeof(uintptr_t)) = 0;
        return VM_STATUS_OK;
    }
    if (
        *(uintptr_t*)vm->data_stack_cur >=
        (*(JSONNodePtr*)(vm->data_stack_cur - sizeof(uintptr_t)))->data
            .array.size) {
        *(JSONNodePtr*)(vm->data_stack_cur - sizeof(uintptr_t)) = 0;
        return VM_STATUS_OK;
    }
    *(JSONNodePtr*)(vm->data_stack_cur - sizeof(uintptr_t)) =
        (*(JSONNodePtr*)(vm->data_stack_cur - sizeof(uintptr_t)))->data
            .array.data[*(uintptr_t*)vm->data_stack_cur];
    return PARSER_STATUS_OK;
}
