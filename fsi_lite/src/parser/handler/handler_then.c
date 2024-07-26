#include "../parser.h"
#include "../../vm/vm.h"
#include "../../util/status.h"

int parser_handler_then(void) {
    if (parser_state & PARSER_STATE_INTERPRET) {
        return PARSER_STATUS_NOT_IN_COMPILATION_MODE;
    }
    return PARSER_STATUS_OK;
}
