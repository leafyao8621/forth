#include <fsi/interpreter/vm.h>
#include <fsi/util/interpreter_handlers.h>

static ForthInterpreterHandler interpreter_handler[] =
    {
        NULL,
        interpreter_handle_print_string,
        interpreter_handle_carriage_return,
        interpreter_handle_push,
        interpreter_handle_print_int,
        interpreter_handle_print_int_unsigned,
        interpreter_handle_call,
        interpreter_handle_return,
        interpreter_handle_add,
        interpreter_handle_sub,
        interpreter_handle_mul,
        interpreter_handle_div,
        interpreter_handle_inc,
        interpreter_handle_dec,
        interpreter_handle_gt,
        interpreter_handle_ugt,
        interpreter_handle_geq,
        interpreter_handle_ugeq,
        interpreter_handle_lt,
        interpreter_handle_ult,
        interpreter_handle_leq,
        interpreter_handle_uleq,
        interpreter_handle_eq,
        interpreter_handle_neq,
        interpreter_handle_jmp,
        interpreter_handle_jzd,
        interpreter_handle_dup,
        interpreter_handle_swap,
        interpreter_handle_drop,
        interpreter_handle_mod,
        interpreter_handle_2pushr,
        interpreter_handle_2popr,
        interpreter_handle_incr,
        interpreter_handle_jgtr,
        interpreter_handle_peekr
    };

ForthVMErr ForthVM_execute(ForthVM *vm) {
    char opcode = 0;
    ForthVMErr err = FORTHVM_ERR_OK;
    for (
        ;;
        vm->ip = (vm->ip & IP_COMPILED) | ((vm->ip & ~IP_COMPILED) + 1)) {
        opcode =
            !(vm->ip & IP_COMPILED) ?
            vm->interpreted.data[vm->ip] :
            vm->compiled.data[vm->ip & ~IP_COMPILED];
        if (opcode == OPCODE_TERMINATE) {
            break;
        }
        err = interpreter_handler[(size_t)opcode](vm);
        if (err) {
            return err;
        }
    }
    return FORTHVM_ERR_OK;
}
