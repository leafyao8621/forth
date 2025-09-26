#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

#include <json/document.h>

int vm_ext_json_node_get_length(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    if (
        (*(JSONNodePtr*)(vm->data_stack_cur - sizeof(uintptr_t)))->type !=
        ARRAY &&
        (*(JSONNodePtr*)(vm->data_stack_cur - sizeof(uintptr_t)))->type !=
        OBJECT) {
        *(JSONNodePtr*)(vm->data_stack_cur - sizeof(uintptr_t)) = 0;
        return VM_STATUS_OK;
    }
    if (
        (*(JSONNodePtr*)(vm->data_stack_cur - sizeof(uintptr_t)))->type ==
        ARRAY) {
        *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) =
            (*(JSONNodePtr*)(vm->data_stack_cur - sizeof(uintptr_t)))->data
                .array.size;
    } else {
        *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) =
            (*(JSONNodePtr*)(vm->data_stack_cur - sizeof(uintptr_t)))->data
                .object.capacity;
    }
    return PARSER_STATUS_OK;
}
