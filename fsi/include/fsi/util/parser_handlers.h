#ifndef UTIL_PARSER_HANDLERS_H_
#define UTIL_PARSER_HANDLERS_H_

#include <fsi/util/errcode.h>
#include <fsi/interpreter/vm.h>
#include <fsi/parser/parser.h>

typedef ForthVMErr (*ForthParserHandler)(ForthParser*, ForthVM*);

ForthVMErr parser_handle_print_string(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_carriage_return(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_print_int(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_start_compilation(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_end_compilation(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_add(ForthParser *parser ,ForthVM *vm);
ForthVMErr parser_handle_subtract(ForthParser *parser ,ForthVM *vm);
ForthVMErr parser_handle_multiply(ForthParser *parser ,ForthVM *vm);
ForthVMErr parser_handle_divide(ForthParser *parser ,ForthVM *vm);
ForthVMErr parser_handle_increment(ForthParser *parser ,ForthVM *vm);
ForthVMErr parser_handle_decrement(ForthParser *parser ,ForthVM *vm);

#endif
