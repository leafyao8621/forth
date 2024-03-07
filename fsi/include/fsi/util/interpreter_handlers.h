#ifndef UTIL_PARSER_HANDLERS_H_
#define UTIL_PARSER_HANDLERS_H_

#include <fsi/util/errcode.h>
#include <fsi/interpreter/vm.h>

typedef ForthVMErr (*ForthInterpreterHandler)(ForthVM*);

ForthVMErr interpreter_handle_print_string(ForthVM *vm);
ForthVMErr interpreter_handle_carriage_return(ForthVM *vm);

#endif
