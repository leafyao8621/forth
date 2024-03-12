#include <fsi/util/parser_handlers.h>
#include <fsi/util/parser_operations.h>

ForthVMErr parser_handle_start_compilation(ForthParser *parser, ForthVM *vm) {
    if (!parser || !vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    parser->state = FORTHPARSER_STATE_DEFINE;
    return FORTHVM_ERR_OK;
}
