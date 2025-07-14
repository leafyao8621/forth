#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

int parser_handler_hello(ForthParser *parser, ForthVM *vm) {
    if (!parser || !vm) {
        return PARSER_STATUS_OK;
    }
    puts("Parsing hello");
    return PARSER_STATUS_OK;
}
