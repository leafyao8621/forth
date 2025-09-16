#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

#include <json/document.h>

int vm_ext_json_document_get_root(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    *(JSONNode**)(vm->data_stack_cur - sizeof(uintptr_t)) =
        (*(JSONDocument**)(vm->data_stack_cur - sizeof(uintptr_t)))->root;
    return PARSER_STATUS_OK;
}
