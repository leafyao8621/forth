#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

#include <http_util/http_request_response.h>

int vm_ext_http_request_parse(ForthVM *vm) {
    if (vm->data_stack_cur - (sizeof(uintptr_t) << 1) < vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return PARSER_STATUS_OK;
    }
    vm->data_stack_cur -= sizeof(uintptr_t);
    int ret =
        HTTPRequest_parse(
            *(HTTPRequest**)(vm->data_stack_cur - sizeof(uintptr_t)),
            *(char**)vm->data_stack_cur
        );
    *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) = ret;
    return PARSER_STATUS_OK;
}
