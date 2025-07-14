#include <fsi/vm/vm.h>
#include <fsi/parser/parser.h>

int vm_ext_hello(ForthVM *vm) {
    if (!vm) {
        return PARSER_STATUS_OK;
    }
    puts("Running hello");
    return PARSER_STATUS_OK;
}
