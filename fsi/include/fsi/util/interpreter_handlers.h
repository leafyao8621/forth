#ifndef UTIL_PARSER_HANDLERS_H_
#define UTIL_PARSER_HANDLERS_H_

#include <fsi/util/errcode.h>
#include <fsi/interpreter/vm.h>

typedef ForthVMErr (*ForthInterpreterHandler)(ForthVM*);

ForthVMErr interpreter_handle_print_string(ForthVM *vm);
ForthVMErr interpreter_handle_carriage_return(ForthVM *vm);
ForthVMErr interpreter_handle_push(ForthVM *vm);
ForthVMErr interpreter_handle_print_int(ForthVM *vm);
ForthVMErr interpreter_handle_print_int_unsigned(ForthVM *vm);
ForthVMErr interpreter_handle_call(ForthVM *vm);
ForthVMErr interpreter_handle_return(ForthVM *vm);
ForthVMErr interpreter_handle_add(ForthVM *vm);
ForthVMErr interpreter_handle_subtract(ForthVM *vm);
ForthVMErr interpreter_handle_multiply(ForthVM *vm);
ForthVMErr interpreter_handle_divide(ForthVM *vm);
ForthVMErr interpreter_handle_increment(ForthVM *vm);
ForthVMErr interpreter_handle_decrement(ForthVM *vm);
ForthVMErr interpreter_handle_gt(ForthVM *vm);
ForthVMErr interpreter_handle_ugt(ForthVM *vm);
ForthVMErr interpreter_handle_geq(ForthVM *vm);
ForthVMErr interpreter_handle_ugeq(ForthVM *vm);
ForthVMErr interpreter_handle_lt(ForthVM *vm);
ForthVMErr interpreter_handle_ult(ForthVM *vm);
ForthVMErr interpreter_handle_leq(ForthVM *vm);
ForthVMErr interpreter_handle_uleq(ForthVM *vm);
ForthVMErr interpreter_handle_eq(ForthVM *vm);
ForthVMErr interpreter_handle_neq(ForthVM *vm);
#endif
