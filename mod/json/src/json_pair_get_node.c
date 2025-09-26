#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

#include <json/document.h>

int vm_ext_json_pair_get_node(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    *(JSONNodePtr*)(vm->data_stack_cur - sizeof(uintptr_t)) =
        (*(HashMapStringJSONNodePtrNode**)
            (vm->data_stack_cur - sizeof(uintptr_t)))
                ->value;
    return PARSER_STATUS_OK;
}
