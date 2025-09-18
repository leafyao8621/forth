#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

#include <json/document.h>

int parser_handler_json_node_type_array(
    ForthParser *parser, char **iter, ForthVM *vm) {
    if (!iter) {
        return PARSER_STATUS_OK;
    }
    if (parser->state & PARSER_STATE_INTERPRET) {
        if (vm->interpreted_cur == vm->interpreted_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(vm->interpreted_cur++) = VM_INSTRUCTION_PUSHD;
        if (vm->interpreted_cur + sizeof(uintptr_t) > vm->interpreted_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_INTERPRETED_OVERFLOW;
        }
        *(uintptr_t*)vm->interpreted_cur = ARRAY;
        vm->interpreted_cur += sizeof(uintptr_t);
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        if (vm->compiled_cur == vm->compiled_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(vm->compiled_cur++) = VM_INSTRUCTION_PUSHD;
        if (vm->compiled_cur + sizeof(uintptr_t) >= vm->compiled_end) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_COMPILED_OVERFLOW;
        }
        *(uintptr_t*)vm->compiled_cur = ARRAY;
        vm->compiled_cur += sizeof(uintptr_t);
    }
    return PARSER_STATUS_OK;
}
