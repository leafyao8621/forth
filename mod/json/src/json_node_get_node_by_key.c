#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

#include <json/document.h>

int vm_ext_json_node_get_node_by_key(ForthVM *vm) {
    if (vm->data_stack_cur - (sizeof(uintptr_t) << 1) < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_DATA_STACK_UNDERFLOW;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    if (
        (*(JSONNodePtr*)(vm->data_stack_cur - sizeof(uintptr_t)))->type !=
        OBJECT) {
        *(JSONNodePtr*)(vm->data_stack_cur - sizeof(uintptr_t)) = 0;
        return VM_STATUS_OK;
    }
    String buf;
    if (DArrayChar_initialize(&buf, strlen(*(char**)vm->data_stack_cur) + 1)) {
        vm->state = VM_STATE_HALTED;
        return VM_STATUS_OUT_OF_MEMORY;
    }
    DArrayChar_push_back_batch(
        &buf,
        (char*)*(char**)vm->data_stack_cur,
        strlen(*(char**)vm->data_stack_cur) + 1
    );
    bool found = false;
    HashMapStringJSONNodePtr_find(
        &(*(JSONNodePtr*)(vm->data_stack_cur - sizeof(uintptr_t)))->data.object,
        &buf,
        &found
    );
    if (!found) {
        DArrayChar_finalize(&buf);
        *(JSONNodePtr*)(vm->data_stack_cur - sizeof(uintptr_t)) = 0;
        return VM_STATUS_OK;
    }
    JSONNodePtr *temp = 0;
    HashMapStringJSONNodePtr_fetch(
        &(*(JSONNodePtr*)(vm->data_stack_cur - sizeof(uintptr_t)))->data.object,
        &buf,
        &temp
    );
    DArrayChar_finalize(&buf);
    *(JSONNodePtr*)(vm->data_stack_cur - sizeof(uintptr_t)) = *temp;
    return PARSER_STATUS_OK;
}
