#include <stdio.h>

#include "vm.h"
#include "handler/handler.h"
#include "../util/status.h"

#define BUILTIN_SIZE 46

static uint8_t mem[65536];

uint8_t *vm_lookup;
uint8_t *vm_lookup_cur;
uint8_t *vm_lookup_end;
uint8_t *vm_literal;
uint8_t *vm_literal_cur;
uint8_t *vm_literal_end;
uint8_t *vm_interpreted;
uint8_t *vm_interpreted_cur;
uint8_t *vm_interpreted_end;
uint8_t *vm_data_stack;
uint8_t *vm_data_stack_cur;
uint8_t *vm_data_stack_end;
uint8_t *vm_control_stack;
uint8_t *vm_control_stack_cur;
uint8_t *vm_control_stack_end;
uint8_t *vm_compiled;
uint8_t *vm_compiled_cur;
uint8_t *vm_compiled_end;
uint8_t *vm_memory;
uint8_t *vm_memory_cur;
uint8_t *vm_memory_end;

uint8_t *vm_ip;
uint8_t vm_state;

void vm_initialize() {
    static const char *builtin[BUILTIN_SIZE] =
        {
            ".",
            "emit",
            "cr",
            ":",
            ";",
            "if",
            "then",
            "else",
            "do",
            "loop",
            "+loop",
            "i",
            "leave",
            "unloop",
            "exit",
            "j",
            "begin",
            "while",
            "repeat",
            "until",
            "again",
            "+",
            "-",
            "*",
            "/",
            "mod",
            "1+",
            "1-",
            "2*",
            "2/",
            "lshift",
            "rshift",
            "and",
            "or",
            "xor",
            "invert",
            "negate",
            ">",
            "<",
            "=",
            "u<",
            "0<",
            "0=",
            "dup",
            "drop",
            "swap"
        };
    const char **iter_builtin = builtin;
    const char *iter_str = 0;
    size_t i = 0;
    vm_lookup = mem;
    vm_lookup_cur = vm_lookup;
    vm_lookup_end = mem + 9500;
    vm_literal = vm_lookup_end;
    vm_literal_cur = vm_literal;
    vm_literal_end = vm_literal + 2000;
    vm_interpreted = vm_literal_end;
    vm_interpreted_cur = vm_interpreted;
    vm_interpreted_end = vm_interpreted + 2000;
    vm_data_stack = vm_interpreted_end;
    vm_data_stack_cur = vm_data_stack;
    vm_data_stack_end = vm_data_stack + 2000;
    vm_control_stack = vm_data_stack_end;
    vm_control_stack_cur = vm_control_stack;
    vm_control_stack_end = vm_control_stack + 2000;
    vm_compiled = vm_control_stack_end;
    vm_compiled_cur = vm_compiled;
    vm_compiled_end = vm_compiled + 24000;
    vm_memory = vm_compiled_end;
    vm_memory_cur = vm_memory;
    vm_memory_end = mem + 65536;
    for (; i < BUILTIN_SIZE; ++i, ++iter_builtin) {
        *(vm_lookup_cur++) = VM_LOOKUP_META_BUILTIN;
        *(size_t*)(vm_lookup_cur) = i;
        vm_lookup_cur += sizeof(size_t);
        for (iter_str = *iter_builtin; *iter_str; ++iter_str, ++vm_lookup_cur) {
            *vm_lookup_cur = *iter_str;
        }
        *(vm_lookup_cur++) = 0;
    }
    *(size_t*)vm_memory_cur = 10;
    vm_memory_cur += sizeof(size_t);
}

void vm_log(void) {
    uint8_t *iter = 0;
    puts("Lookup:");
    for (iter = vm_lookup; iter < vm_lookup_cur; ++iter) {
        printf("0x%016lX ", (uintptr_t)iter);
        if (*iter & VM_LOOKUP_META_BUILTIN) {
            printf("%s ", "BUILTIN");
        }
        if (*iter & VM_LOOKUP_META_CALL) {
            printf("%s ", "CALL");
        }
        printf("%016lX ", *(size_t*)(++iter));
        iter += sizeof(size_t);
        for (; *iter; ++iter) {
            putchar(*iter);
        }
        putchar(10);
    }
    puts("Compiled:");
    for (iter = vm_compiled; iter < vm_compiled_cur; ++iter) {
        printf("0x%016lX ", (uintptr_t)iter);
        switch (*iter) {
        case VM_INSTRUCTION_HALT:
            printf("%s", "HALT");
            break;
        case VM_INSTRUCTION_RET:
            printf("%s", "RET");
            break;
        case VM_INSTRUCTION_PUSHD:
            printf("%s 0x%016lX", "PUSHD", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_PINT:
            printf("%s", "PINT");
            break;
        case VM_INSTRUCTION_EMIT:
            printf("%s", "EMIT");
            break;
        case VM_INSTRUCTION_CALL:
            printf("%s 0x%016lX", "CALL", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_JZD:
            printf("%s 0x%016lX", "JZD", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_JNZD:
            printf("%s 0x%016lX", "JNZD", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_JMP:
            printf("%s 0x%016lX", "JMP", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_2PUSHC:
            printf("%s", "2PUSHC");
            break;
        case VM_INSTRUCTION_PEEKC:
            printf("%s", "PEEKC");
            break;
        case VM_INSTRUCTION_2RMC:
            printf("%s", "2RMC");
            break;
        case VM_INSTRUCTION_JNEC:
            printf("%s 0x%016lX", "JNEC", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_INCC:
            printf("%s", "INCC");
            break;
        case VM_INSTRUCTION_ADDC:
            printf("%s", "ADDC");
            break;
        case VM_INSTRUCTION_3PEEKC:
            printf("%s", "3PEEKC");
            break;
        case VM_INSTRUCTION_ADDD:
            printf("%s", "ADDD");
            break;
        case VM_INSTRUCTION_SUBD:
            printf("%s", "SUBD");
            break;
        case VM_INSTRUCTION_MULTD:
            printf("%s", "MULTD");
            break;
        case VM_INSTRUCTION_DIVD:
            printf("%s", "DIVD");
            break;
        case VM_INSTRUCTION_MODD:
            printf("%s", "MODD");
            break;
        case VM_INSTRUCTION_INCD:
            printf("%s", "INCD");
            break;
        case VM_INSTRUCTION_DECD:
            printf("%s", "DECD");
            break;
        case VM_INSTRUCTION_DUBD:
            printf("%s", "DUBD");
            break;
        case VM_INSTRUCTION_HALFD:
            printf("%s", "HALFD");
            break;
        case VM_INSTRUCTION_LSLD:
            printf("%s", "LSLD");
            break;
        case VM_INSTRUCTION_LSRD:
            printf("%s", "LSRD");
            break;
        case VM_INSTRUCTION_ANDD:
            printf("%s", "ANDD");
            break;
        case VM_INSTRUCTION_ORD:
            printf("%s", "ORD");
            break;
        case VM_INSTRUCTION_XORD:
            printf("%s", "XORD");
            break;
        case VM_INSTRUCTION_NOTD:
            printf("%s", "NOTD");
            break;
        case VM_INSTRUCTION_NEGD:
            printf("%s", "NEGD");
            break;
        case VM_INSTRUCTION_GTD:
            printf("%s", "GTD");
            break;
        case VM_INSTRUCTION_LTD:
            printf("%s", "LTD");
            break;
        case VM_INSTRUCTION_EQD:
            printf("%s", "EQD");
            break;
        case VM_INSTRUCTION_ULTD:
            printf("%s", "ULTD");
            break;
        case VM_INSTRUCTION_LTZD:
            printf("%s", "LTZD");
            break;
        case VM_INSTRUCTION_EQZD:
            printf("%s", "EQZD");
            break;
        case VM_INSTRUCTION_DUPD:
            printf("%s", "DUPD");
            break;
        case VM_INSTRUCTION_DROPD:
            printf("%s", "DROPD");
            break;
        case VM_INSTRUCTION_SWAPD:
            printf("%s", "SWAPD");
            break;
        }
        putchar(10);
    }
    puts("Interpreted:");
    for (iter = vm_interpreted; iter < vm_interpreted_cur; ++iter) {
        printf("0x%016lX ", (uintptr_t)iter);
        switch (*iter) {
        case VM_INSTRUCTION_HALT:
            printf("%s", "HALT");
            break;
        case VM_INSTRUCTION_RET:
            printf("%s", "RET");
            break;
        case VM_INSTRUCTION_PUSHD:
            printf("%s 0x%016lX", "PUSHD", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_PINT:
            printf("%s", "PINT");
            break;
        case VM_INSTRUCTION_EMIT:
            printf("%s", "EMIT");
            break;
        case VM_INSTRUCTION_CALL:
            printf("%s 0x%016lX", "CALL", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_ADDD:
            printf("%s", "ADDD");
            break;
        case VM_INSTRUCTION_SUBD:
            printf("%s", "SUBD");
            break;
        case VM_INSTRUCTION_MULTD:
            printf("%s", "MULTD");
            break;
        case VM_INSTRUCTION_DIVD:
            printf("%s", "DIVD");
            break;
        case VM_INSTRUCTION_MODD:
            printf("%s", "MODD");
            break;
        case VM_INSTRUCTION_INCD:
            printf("%s", "INCD");
            break;
        case VM_INSTRUCTION_DECD:
            printf("%s", "DECD");
            break;
        case VM_INSTRUCTION_DUBD:
            printf("%s", "DUBD");
            break;
        case VM_INSTRUCTION_HALFD:
            printf("%s", "HALFD");
            break;
        case VM_INSTRUCTION_LSLD:
            printf("%s", "LSLD");
            break;
        case VM_INSTRUCTION_LSRD:
            printf("%s", "LSRD");
            break;
        case VM_INSTRUCTION_ANDD:
            printf("%s", "ANDD");
            break;
        case VM_INSTRUCTION_ORD:
            printf("%s", "ORD");
            break;
        case VM_INSTRUCTION_XORD:
            printf("%s", "XORD");
            break;
        case VM_INSTRUCTION_NOTD:
            printf("%s", "NOTD");
            break;
        case VM_INSTRUCTION_NEGD:
            printf("%s", "NEGD");
            break;
        case VM_INSTRUCTION_GTD:
            printf("%s", "GTD");
            break;
        case VM_INSTRUCTION_LTD:
            printf("%s", "LTD");
            break;
        case VM_INSTRUCTION_EQD:
            printf("%s", "EQD");
            break;
        case VM_INSTRUCTION_ULTD:
            printf("%s", "ULTD");
            break;
        case VM_INSTRUCTION_LTZD:
            printf("%s", "LTZD");
            break;
        case VM_INSTRUCTION_EQZD:
            printf("%s", "EQZD");
            break;
        case VM_INSTRUCTION_DUPD:
            printf("%s", "DUPD");
            break;
        case VM_INSTRUCTION_DROPD:
            printf("%s", "DROPD");
            break;
        case VM_INSTRUCTION_SWAPD:
            printf("%s", "SWAPD");
            break;
        }
        putchar(10);
    }
}

void vm_reset(void) {
    vm_interpreted_cur = vm_interpreted;
    vm_ip = vm_interpreted;
}

int vm_run(bool debug) {
    int ret = VM_STATUS_OK;
    vm_state = VM_STATE_RUNNING;
    for (; vm_state != VM_STATE_HALTED; ++vm_ip) {
        if (debug) {
            printf("\nExecuting 0x%016lX ", (uintptr_t)vm_ip);
            switch (*vm_ip) {
            case VM_INSTRUCTION_HALT:
                printf("%s", "HALT");
                break;
            case VM_INSTRUCTION_RET:
                printf("%s", "RET");
                break;
            case VM_INSTRUCTION_PUSHD:
                printf("%s 0x%016lX", "PUSHD", *(uintptr_t*)(vm_ip + 1));
                break;
            case VM_INSTRUCTION_PINT:
                printf("%s", "PINT");
                break;
            case VM_INSTRUCTION_EMIT:
                printf("%s", "EMIT");
                break;
            case VM_INSTRUCTION_CALL:
                printf("%s 0x%016lX", "CALL", *(uintptr_t*)(vm_ip + 1));
                break;
            case VM_INSTRUCTION_JZD:
                printf("%s 0x%016lX", "JZD", *(uintptr_t*)(vm_ip + 1));
                break;
            case VM_INSTRUCTION_JNZD:
                printf("%s 0x%016lX", "JNZD", *(uintptr_t*)(vm_ip + 1));
                break;
            case VM_INSTRUCTION_JMP:
                printf("%s 0x%016lX", "JMP", *(uintptr_t*)(vm_ip + 1));
                break;
            case VM_INSTRUCTION_2PUSHC:
                printf("%s", "2PUSHC");
                break;
            case VM_INSTRUCTION_PEEKC:
                printf("%s", "PEEKC");
                break;
            case VM_INSTRUCTION_2RMC:
                printf("%s", "2RMC");
                break;
            case VM_INSTRUCTION_JNEC:
                printf("%s 0x%016lX", "JNEC", *(uintptr_t*)(vm_ip + 1));
                break;
            case VM_INSTRUCTION_INCC:
                printf("%s", "INCC");
                break;
            case VM_INSTRUCTION_ADDC:
                printf("%s", "ADDC");
                break;
            case VM_INSTRUCTION_3PEEKC:
                printf("%s", "3PEEKC");
                break;
            case VM_INSTRUCTION_ADDD:
                printf("%s", "ADDD");
                break;
            case VM_INSTRUCTION_SUBD:
                printf("%s", "SUBD");
                break;
            case VM_INSTRUCTION_MULTD:
                printf("%s", "MULTD");
                break;
            case VM_INSTRUCTION_DIVD:
                printf("%s", "DIVD");
                break;
            case VM_INSTRUCTION_MODD:
                printf("%s", "MODD");
                break;
            case VM_INSTRUCTION_INCD:
                printf("%s", "INCD");
                break;
            case VM_INSTRUCTION_DECD:
                printf("%s", "DECD");
                break;
            case VM_INSTRUCTION_DUBD:
                printf("%s", "DUBD");
                break;
            case VM_INSTRUCTION_HALFD:
                printf("%s", "HALFD");
                break;
            case VM_INSTRUCTION_LSLD:
                printf("%s", "LSLD");
                break;
            case VM_INSTRUCTION_LSRD:
                printf("%s", "LSRD");
                break;
            case VM_INSTRUCTION_ANDD:
                printf("%s", "ANDD");
                break;
            case VM_INSTRUCTION_ORD:
                printf("%s", "ORD");
                break;
            case VM_INSTRUCTION_XORD:
                printf("%s", "XORD");
                break;
            case VM_INSTRUCTION_NOTD:
                printf("%s", "NOTD");
                break;
            case VM_INSTRUCTION_NEGD:
                printf("%s", "NEGD");
                break;
            case VM_INSTRUCTION_GTD:
                printf("%s", "GTD");
                break;
            case VM_INSTRUCTION_LTD:
                printf("%s", "LTD");
                break;
            case VM_INSTRUCTION_EQD:
                printf("%s", "EQD");
                break;
            case VM_INSTRUCTION_ULTD:
                printf("%s", "ULTD");
                break;
            case VM_INSTRUCTION_LTZD:
                printf("%s", "LTZD");
                break;
            case VM_INSTRUCTION_EQZD:
                printf("%s", "EQZD");
                break;
            case VM_INSTRUCTION_DUPD:
                printf("%s", "DUPD");
                break;
            case VM_INSTRUCTION_DROPD:
                printf("%s", "DROPD");
                break;
            case VM_INSTRUCTION_SWAPD:
                printf("%s", "SWAPD");
                break;
            }
            putchar(10);
        }
        switch (*vm_ip) {
        case VM_INSTRUCTION_HALT:
            vm_state = VM_STATE_HALTED;
            break;
        case VM_INSTRUCTION_RET:
            ret = vm_handler_ret();
            break;
        case VM_INSTRUCTION_PUSHD:
            ret = vm_handler_pushd();
            break;
        case VM_INSTRUCTION_PINT:
            ret = vm_handler_pint();
            break;
        case VM_INSTRUCTION_EMIT:
            ret = vm_handler_emit();
            break;
        case VM_INSTRUCTION_CALL:
            ret = vm_handler_call();
            break;
        case VM_INSTRUCTION_JZD:
            ret = vm_handler_jzd();
            break;
        case VM_INSTRUCTION_JNZD:
            ret = vm_handler_jnzd();
            break;
        case VM_INSTRUCTION_JMP:
            ret = vm_handler_jmp();
            break;
        case VM_INSTRUCTION_2PUSHC:
            ret = vm_handler_2pushc();
            break;
        case VM_INSTRUCTION_PEEKC:
            ret = vm_handler_peekc();
            break;
        case VM_INSTRUCTION_2RMC:
            ret = vm_handler_2rmc();
            break;
        case VM_INSTRUCTION_JNEC:
            ret = vm_handler_jnec();
            break;
        case VM_INSTRUCTION_INCC:
            ret = vm_handler_incc();
            break;
        case VM_INSTRUCTION_ADDC:
            ret = vm_handler_addc();
            break;
        case VM_INSTRUCTION_3PEEKC:
            ret = vm_handler_3peekc();
            break;
        case VM_INSTRUCTION_ADDD:
            ret = vm_handler_addd();
            break;
        case VM_INSTRUCTION_SUBD:
            ret = vm_handler_subd();
            break;
        case VM_INSTRUCTION_MULTD:
            ret = vm_handler_multd();
            break;
        case VM_INSTRUCTION_DIVD:
            ret = vm_handler_divd();
            break;
        case VM_INSTRUCTION_INCD:
            ret = vm_handler_incd();
            break;
        case VM_INSTRUCTION_MODD:
            ret = vm_handler_modd();
            break;
        case VM_INSTRUCTION_DECD:
            ret = vm_handler_decd();
            break;
        case VM_INSTRUCTION_DUBD:
            ret = vm_handler_dubd();
            break;
        case VM_INSTRUCTION_HALFD:
            ret = vm_handler_halfd();
            break;
        case VM_INSTRUCTION_LSLD:
            ret = vm_handler_lsld();
            break;
        case VM_INSTRUCTION_LSRD:
            ret = vm_handler_lsrd();
            break;
        case VM_INSTRUCTION_ANDD:
            ret = vm_handler_andd();
            break;
        case VM_INSTRUCTION_ORD:
            ret = vm_handler_ord();
            break;
        case VM_INSTRUCTION_XORD:
            ret = vm_handler_xord();
            break;
        case VM_INSTRUCTION_NOTD:
            ret = vm_handler_notd();
            break;
        case VM_INSTRUCTION_NEGD:
            ret = vm_handler_negd();
            break;
        case VM_INSTRUCTION_GTD:
            ret = vm_handler_gtd();
            break;
        case VM_INSTRUCTION_LTD:
            ret = vm_handler_ltd();
            break;
        case VM_INSTRUCTION_EQD:
            ret = vm_handler_eqd();
            break;
        case VM_INSTRUCTION_ULTD:
            ret = vm_handler_ultd();
            break;
        case VM_INSTRUCTION_LTZD:
            ret = vm_handler_ltzd();
            break;
        case VM_INSTRUCTION_EQZD:
            ret = vm_handler_eqzd();
            break;
        case VM_INSTRUCTION_DUPD:
            ret = vm_handler_dupd();
            break;
        case VM_INSTRUCTION_DROPD:
            ret = vm_handler_dropd();
            break;
        case VM_INSTRUCTION_SWAPD:
            ret = vm_handler_swapd();
            break;
        }
    }
    return ret;
}
