#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_dot_double_quote(
    ForthParser *parser, char **iter, ForthVM *vm) {
    int ret = parser_handler_s_double_quote(parser, iter, vm);
    if (ret) {
        return ret;
    }
    return parser_handler_type(parser, vm);
}
