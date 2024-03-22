#ifndef UTIL_PARSER_HANDLERS_H_
#define UTIL_PARSER_HANDLERS_H_

#include <fsi/util/errcode.h>
#include <fsi/interpreter/vm.h>
#include <fsi/parser/parser.h>

typedef ForthVMErr (*ForthParserHandler)(ForthParser*, ForthVM*);

ForthVMErr parser_handle_print_string(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_carriage_return(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_print_int(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_print_int_unsigned(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_start_compilation(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_end_compilation(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_add(ForthParser *parser ,ForthVM *vm);
ForthVMErr parser_handle_subtract(ForthParser *parser ,ForthVM *vm);
ForthVMErr parser_handle_multiply(ForthParser *parser ,ForthVM *vm);
ForthVMErr parser_handle_divide(ForthParser *parser ,ForthVM *vm);
ForthVMErr parser_handle_increment(ForthParser *parser ,ForthVM *vm);
ForthVMErr parser_handle_decrement(ForthParser *parser ,ForthVM *vm);
ForthVMErr parser_handle_gt(ForthParser *parser ,ForthVM *vm);
ForthVMErr parser_handle_ugt(ForthParser *parser ,ForthVM *vm);
ForthVMErr parser_handle_geq(ForthParser *parser ,ForthVM *vm);
ForthVMErr parser_handle_ugeq(ForthParser *parser ,ForthVM *vm);
ForthVMErr parser_handle_lt(ForthParser *parser ,ForthVM *vm);
ForthVMErr parser_handle_ult(ForthParser *parser ,ForthVM *vm);
ForthVMErr parser_handle_leq(ForthParser *parser ,ForthVM *vm);
ForthVMErr parser_handle_uleq(ForthParser *parser ,ForthVM *vm);
ForthVMErr parser_handle_eq(ForthParser *parser ,ForthVM *vm);
ForthVMErr parser_handle_neq(ForthParser *parser ,ForthVM *vm);

#endif
