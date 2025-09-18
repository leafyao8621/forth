#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

#include <json/document.h>

int vm_ext_json_node_get_number(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    if (vm->float_stack_cur == vm->float_stack_end) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_FLOAT_STACK_OVERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    *(double*)vm->float_stack_cur =
        (*(JSONNode**)vm->data_stack_cur)->data.number;
    vm->float_stack_cur += sizeof(uintptr_t);
    return PARSER_STATUS_OK;
}
