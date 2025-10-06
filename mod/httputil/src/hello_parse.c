#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

int parser_handler_hello(ForthParser *parser, char **iter, ForthVM *vm) {
    if (!parser || !vm || !iter) {
        return PARSER_STATUS_OK;
    }
    puts("Parsing hello");
    return PARSER_STATUS_OK;
}
