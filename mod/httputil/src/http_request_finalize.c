#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

#include <http_util/http_request_response.h>

int vm_ext_http_request_finalize(ForthVM *vm) {
    if (vm->data_stack_cur == vm->data_stack) {
        vm->state = VM_STATE_HALTED;
        return PARSER_STATUS_OK;
    }
    int ret =
        HTTPRequest_finalize(
            *(HTTPRequest**)(vm->data_stack_cur - sizeof(uintptr_t))
        );
    *(uintptr_t*)(vm->data_stack_cur - sizeof(uintptr_t)) = ret;
    return PARSER_STATUS_OK;
}
