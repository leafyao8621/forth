#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

#include <json/document.h>

int vm_ext_json_pair_get_in_use(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) =
        (*(HashMapStringJSONNodePtrNode**)
            (vm->data_stack_cur - sizeof(uintptr_t)))
                ->in_use ?
        -1 :
        0;
    return PARSER_STATUS_OK;
}
