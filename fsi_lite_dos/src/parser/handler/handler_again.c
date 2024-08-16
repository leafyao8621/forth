#include "src\parser\parser.h"
#include "src\vm\vm.h"
#include "src\util\status.h"

int parser_handler_again(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    if (
        parser_loop_stack_cur - (sizeof(uintptr_t) + 1) <
        parser_loop_stack) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_UNDERFLOW;
    }
    parser_loop_stack_cur -= (sizeof(uintptr_t) + 1);
    if (*parser_loop_stack_cur != PARSER_CONTROL_BEGIN) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_PARSER_CONTROL_STACK_MISMATCH;
    }
    if (vm_compiled_cur == vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    *(vm_compiled_cur++) = VM_INSTRUCTION_JMP;
    *(uint8_t**)(vm_compiled_cur) =
        *(uint8_t**)(parser_loop_stack_cur + 1) - 1;
    if (vm_compiled_cur + sizeof(uintptr_t) > vm_compiled_end) {
        parser_status = PARSER_STATUS_END;
        return PARSER_STATUS_COMPILED_OVERFLOW;
    }
    vm_compiled_cur += sizeof(uintptr_t);
    return PARSER_STATUS_OK;
}
