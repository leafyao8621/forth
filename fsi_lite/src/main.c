#include "vm/vm.h"
#include "parser/parser.h"

int main(void) {
    vm_initialize();
    parser_parse(stdin);
    return 0;
}
