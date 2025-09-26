#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

#include <json/document.h>

int vm_ext_json_pair_get_key(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    *(char**)(vm->data_stack_cur - sizeof(uintptr_t)) =
        (*(HashMapStringJSONNodePtrNode**)
            (vm->data_stack_cur - sizeof(uintptr_t)))
                ->key.data;
    return PARSER_STATUS_OK;
}
