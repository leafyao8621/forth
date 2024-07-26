#include "../parser.h"
#include "../../vm/vm.h"
#include "../../util/status.h"

int parser_handler_colon(void) {
    if (parser_state & PARSER_STATE_COMPILE) {
        return PARSER_STATUS_NESTED_COMPILATION_MODE;
    }
    parser_state ^= PARSER_STATE_INTERPRET;
    parser_state |= PARSER_STATE_COMPILE;
    parser_state |= PARSER_STATE_NAME;
    return PARSER_STATUS_OK;
}
