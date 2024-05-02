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
ForthVMErr parser_handle_add(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_subtract(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_multiply(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_divide(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_increment(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_decrement(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_gt(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_ugt(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_geq(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_ugeq(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_lt(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_ult(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_leq(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_uleq(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_eq(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_neq(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_if(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_else(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_then(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_dup(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_swap(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_drop(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_mod(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_do(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_loop(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_i(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_begin(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_while(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_repeat(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_variable(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_load(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_store(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_create(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_allot(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_cells(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_loadc(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_storec(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_pushm(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_pushmc(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_emit(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_2dup(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_and(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_or(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_xor(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_invert(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_lshift(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_rshift(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_true(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_false(ForthParser *parser, ForthVM *vm);
ForthVMErr parser_handle_negate(ForthParser *parser, ForthVM *vm);

#endif
