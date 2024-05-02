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
ForthVMErr interpreter_handle_sub(ForthVM *vm);
ForthVMErr interpreter_handle_mul(ForthVM *vm);
ForthVMErr interpreter_handle_div(ForthVM *vm);
ForthVMErr interpreter_handle_inc(ForthVM *vm);
ForthVMErr interpreter_handle_dec(ForthVM *vm);
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
ForthVMErr interpreter_handle_jmp(ForthVM *vm);
ForthVMErr interpreter_handle_jzd(ForthVM *vm);
ForthVMErr interpreter_handle_dup(ForthVM *vm);
ForthVMErr interpreter_handle_swap(ForthVM *vm);
ForthVMErr interpreter_handle_drop(ForthVM *vm);
ForthVMErr interpreter_handle_mod(ForthVM *vm);
ForthVMErr interpreter_handle_2pushr(ForthVM *vm);
ForthVMErr interpreter_handle_2popr(ForthVM *vm);
ForthVMErr interpreter_handle_incr(ForthVM *vm);
ForthVMErr interpreter_handle_jgtr(ForthVM *vm);
ForthVMErr interpreter_handle_peekr(ForthVM *vm);
ForthVMErr interpreter_handle_pushw(ForthVM *vm);
ForthVMErr interpreter_handle_dw(ForthVM *vm);
ForthVMErr interpreter_handle_allotc(ForthVM *vm);
ForthVMErr interpreter_handle_ld(ForthVM *vm);
ForthVMErr interpreter_handle_st(ForthVM *vm);
ForthVMErr interpreter_handle_allot(ForthVM *vm);
ForthVMErr interpreter_handle_msz(ForthVM *vm);
ForthVMErr interpreter_handle_ldc(ForthVM *vm);
ForthVMErr interpreter_handle_stc(ForthVM *vm);
ForthVMErr interpreter_handle_pushm(ForthVM *vm);
ForthVMErr interpreter_handle_pushmc(ForthVM *vm);
ForthVMErr interpreter_handle_emit(ForthVM *vm);
ForthVMErr interpreter_handle_2dup(ForthVM *vm);
ForthVMErr interpreter_handle_and(ForthVM *vm);
ForthVMErr interpreter_handle_or(ForthVM *vm);
ForthVMErr interpreter_handle_xor(ForthVM *vm);
ForthVMErr interpreter_handle_inv(ForthVM *vm);
ForthVMErr interpreter_handle_lsl(ForthVM *vm);
ForthVMErr interpreter_handle_lsr(ForthVM *vm);
ForthVMErr interpreter_handle_neg(ForthVM *vm);
ForthVMErr interpreter_handle_prevr(ForthVM *vm);

#endif
