#include <fsi/interpreter/vm.h>
#include <fsi/util/interpreter_handlers.h>

ForthVMErr ForthVM_execute(ForthVM *vm) {
    char opcode = 0;
    ForthVMErr err = FORTHVM_ERR_OK;
    if (!vm->interpreted.size) {
        return FORTHVM_ERR_OK;
    }
    vm->ip = 0;
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
        switch (opcode) {
        case OPCODE_PRINT_STRING:
            err = interpreter_handle_print_string(vm);
            break;
        case OPCODE_CARRIAGE_RETURN:
            err = interpreter_handle_carriage_return(vm);
            break;
        case OPCODE_PUSH:
            err = interpreter_handle_push(vm);
            break;
        case OPCODE_PRINT_INT:
            err = interpreter_handle_print_int(vm);
            break;
        case OPCODE_PRINT_INT_UNSIGNED:
            err = interpreter_handle_print_int_unsigned(vm);
            break;
        case OPCODE_CALL:
            err = interpreter_handle_call(vm);
            break;
        case OPCODE_RETURN:
            err = interpreter_handle_return(vm);
            break;
        case OPCODE_ADD:
            err = interpreter_handle_add(vm);
            break;
        case OPCODE_SUB:
            err = interpreter_handle_sub(vm);
            break;
        case OPCODE_MUL:
            err = interpreter_handle_mul(vm);
            break;
        case OPCODE_DIV:
            err = interpreter_handle_div(vm);
            break;
        case OPCODE_INC:
            err = interpreter_handle_inc(vm);
            break;
        case OPCODE_DEC:
            err = interpreter_handle_dec(vm);
            break;
        case OPCODE_GT:
            err = interpreter_handle_gt(vm);
            break;
        case OPCODE_UGT:
            err = interpreter_handle_ugt(vm);
            break;
        case OPCODE_GEQ:
            err = interpreter_handle_geq(vm);
            break;
        case OPCODE_UGEQ:
            err = interpreter_handle_ugeq(vm);
            break;
        case OPCODE_LT:
            err = interpreter_handle_lt(vm);
            break;
        case OPCODE_ULT:
            err = interpreter_handle_ult(vm);
            break;
        case OPCODE_LEQ:
            err = interpreter_handle_leq(vm);
            break;
        case OPCODE_ULEQ:
            err = interpreter_handle_uleq(vm);
            break;
        case OPCODE_EQ:
            err = interpreter_handle_eq(vm);
            break;
        case OPCODE_NEQ:
            err = interpreter_handle_neq(vm);
            break;
        case OPCODE_JMP:
            err = interpreter_handle_jmp(vm);
            break;
        case OPCODE_JZD:
            err = interpreter_handle_jzd(vm);
            break;
        case OPCODE_DUP:
            err = interpreter_handle_dup(vm);
            break;
        case OPCODE_SWAP:
            err = interpreter_handle_swap(vm);
            break;
        case OPCODE_DROP:
            err = interpreter_handle_drop(vm);
            break;
        case OPCODE_MOD:
            err = interpreter_handle_mod(vm);
            break;
        case OPCODE_2PUSHR:
            err = interpreter_handle_2pushr(vm);
            break;
        case OPCODE_2POPR:
            err = interpreter_handle_2popr(vm);
            break;
        case OPCODE_INCR:
            err = interpreter_handle_incr(vm);
            break;
        case OPCODE_JGTR:
            err = interpreter_handle_jgtr(vm);
            break;
        case OPCODE_PEEKR:
            err = interpreter_handle_peekr(vm);
            break;
        case OPCODE_PUSHW:
            err = interpreter_handle_pushw(vm);
            break;
        case OPCODE_DW:
            err = interpreter_handle_dw(vm);
            break;
        case OPCODE_ALLOTC:
            err = interpreter_handle_allotc(vm);
            break;
        case OPCODE_LD:
            err = interpreter_handle_ld(vm);
            break;
        case OPCODE_ST:
            err = interpreter_handle_st(vm);
            break;
        case OPCODE_ALLOT:
            err = interpreter_handle_allot(vm);
            break;
        case OPCODE_MSZ:
            err = interpreter_handle_msz(vm);
            break;
        case OPCODE_LDC:
            err = interpreter_handle_ldc(vm);
            break;
        case OPCODE_STC:
            err = interpreter_handle_stc(vm);
            break;
        case OPCODE_PUSHM:
            err = interpreter_handle_pushm(vm);
            break;
        case OPCODE_PUSHMC:
            err = interpreter_handle_pushmc(vm);
            break;
        case OPCODE_EMIT:
            err = interpreter_handle_emit(vm);
            break;
        case OPCODE_2DUP:
            err = interpreter_handle_2dup(vm);
            break;
        case OPCODE_AND:
            err = interpreter_handle_and(vm);
            break;
        case OPCODE_OR:
            err = interpreter_handle_or(vm);
            break;
        case OPCODE_XOR:
            err = interpreter_handle_xor(vm);
            break;
        case OPCODE_INV:
            err = interpreter_handle_inv(vm);
            break;
        case OPCODE_LSL:
            err = interpreter_handle_lsl(vm);
            break;
        case OPCODE_LSR:
            err = interpreter_handle_lsr(vm);
            break;
        case OPCODE_NEG:
            err = interpreter_handle_neg(vm);
            break;
        case OPCODE_PREVR:
            err = interpreter_handle_prevr(vm);
            break;
        default:
            return FORTHVM_ERR_INVALID_OPCODE;
        }
        if (err) {
            return err;
        }
    }
    DArrayChar_clear(&vm->interpreted);
    return FORTHVM_ERR_OK;
}
