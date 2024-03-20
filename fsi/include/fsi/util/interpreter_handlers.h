#ifndef UTIL_PARSER_HANDLERS_H_
#define UTIL_PARSER_HANDLERS_H_

#include <fsi/util/errcode.h>
#include <fsi/interpreter/vm.h>

typedef ForthVMErr (*ForthInterpreterHandler)(ForthVM*);

ForthVMErr interpreter_handle_print_string(ForthVM *vm);
ForthVMErr interpreter_handle_carriage_return(ForthVM *vm);
ForthVMErr interpreter_handle_push(ForthVM *vm);
ForthVMErr interpreter_handle_print_int(ForthVM *vm);
ForthVMErr interpreter_handle_call(ForthVM *vm);
ForthVMErr interpreter_handle_return(ForthVM *vm);
ForthVMErr interpreter_handle_add(ForthVM *vm);
ForthVMErr interpreter_handle_subtract(ForthVM *vm);
ForthVMErr interpreter_handle_multiply(ForthVM *vm);
ForthVMErr interpreter_handle_divide(ForthVM *vm);
ForthVMErr interpreter_handle_increment(ForthVM *vm);
ForthVMErr interpreter_handle_decrement(ForthVM *vm);

#endif
