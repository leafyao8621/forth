#include <stdio.h>
#include <stdlib.h>

#include <fsi/vm/vm.h>
#include <fsi/util/status.h>
#include "handler/handler.h"


#define BUILTIN_SIZE 56
#define MEMORY_SIZE 2

ForthVMStatus vm_initialize(
    ForthVM *vm,
    size_t memory,
    size_t lookup,
    size_t literal,
    size_t interpreted,
    size_t data_stack,
    size_t control_stack,
    size_t compiled) {
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
            "create",
            "variable",
            "allot",
            "cells",
            "@",
            "!",
            "c@",
            "c!",
            ",",
            "c,",
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
    static const char *memory_symbol[MEMORY_SIZE] =
        {
            "base",
            "here"
        };
    static const uintptr_t memory_value[MEMORY_SIZE] =
        {
            10,
            0
        };
    vm->mem = calloc(memory, 1);
    if (!vm->mem) {
        return VM_STATUS_OUT_OF_MEMORY;
    }
    vm->lookup = vm->mem;
    vm->lookup_cur = vm->lookup;
    vm->lookup_end = vm->mem + lookup;
    vm->literal = vm->lookup_end;
    vm->literal_cur = vm->literal;
    vm->literal_end = vm->literal + literal;
    vm->interpreted = vm->literal_end;
    vm->interpreted_cur = vm->interpreted;
    vm->interpreted_end = vm->interpreted + interpreted;
    vm->data_stack = vm->interpreted_end;
    vm->data_stack_cur = vm->data_stack;
    vm->data_stack_end = vm->data_stack + data_stack;
    vm->control_stack = vm->data_stack_end;
    vm->control_stack_cur = vm->control_stack;
    vm->control_stack_end = vm->control_stack + control_stack;
    vm->compiled = vm->control_stack_end;
    vm->compiled_cur = vm->compiled;
    vm->compiled_end = vm->compiled + compiled;
    vm->memory = vm->compiled_end;
    vm->memory_cur = vm->memory + sizeof(size_t);
    *(uint8_t**)vm->memory_cur = vm->memory;
    vm->memory_end = vm->mem + memory;
    const char **iter_builtin = builtin;
    const char *iter_str = 0;
    const char **iter_memory = memory_symbol;
    const uintptr_t *iter_memory_value = memory_value;
    for (size_t i = 0; i < BUILTIN_SIZE; ++i, ++iter_builtin) {
        *(vm->lookup_cur++) = VM_LOOKUP_META_BUILTIN;
        *(size_t*)(vm->lookup_cur) = i;
        vm->lookup_cur += sizeof(size_t);
        for (
            iter_str = *iter_builtin; *iter_str; ++iter_str, ++vm->lookup_cur) {
            *vm->lookup_cur = *iter_str;
        }
        *(vm->lookup_cur++) = 0;
    }
    for (
        size_t i = 0;
        i < MEMORY_SIZE;
        ++i, ++iter_memory, ++iter_memory_value) {
        *(vm->lookup_cur++) = VM_LOOKUP_META_MEMORY;
        *(uintptr_t*)(vm->lookup_cur) = *(uintptr_t*)vm->memory_cur;
        if (i != 1) {
            **(uintptr_t**)vm->memory_cur = *iter_memory_value;
        }
        vm->lookup_cur += sizeof(size_t);
        *vm->memory_cur += sizeof(size_t);
        for (iter_str = *iter_memory; *iter_str; ++iter_str, ++vm->lookup_cur) {
            *vm->lookup_cur = *iter_str;
        }
        *(vm->lookup_cur++) = 0;
    }
    return VM_STATUS_OK;
}

void vm_finalize(ForthVM *vm) {
    free(vm->mem);
}

void vm_log(ForthVM *vm) {
    puts("Lookup:");
    for (uint8_t *iter = vm->lookup; iter < vm->lookup_cur; ++iter) {
        printf("0x%016lX ", (uintptr_t)iter);
        if (*iter & VM_LOOKUP_META_BUILTIN) {
            printf("%s ", "BUILTIN");
        }
        if (*iter & VM_LOOKUP_META_CALL) {
            printf("%s ", "CALL");
        }
        if (*iter & VM_LOOKUP_META_MEMORY) {
            printf("%s ", "MEMORY");
        }
        if (*iter & VM_LOOKUP_META_CREATE) {
            printf("%s ", "CREATE");
        }
        printf("0x%016lX ", *(size_t*)(++iter));
        iter += sizeof(size_t);
        for (; *iter; ++iter) {
            putchar(*iter);
        }
        putchar(10);
    }
    puts("Memory:");
    for (
        uint8_t *iter = vm->memory;
        iter < *(uint8_t**)vm->memory_cur;
        ++iter) {
        printf("0x%016lX 0x%02hhX\n", (uintptr_t)iter, *iter);
    }
    puts("Compiled:");
    for (uint8_t *iter = vm->compiled; iter < vm->compiled_cur; ++iter) {
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
        case VM_INSTRUCTION_PUSHID:
            printf("%s 0x%016lX", "PUSHID", *(uintptr_t*)(iter + 1));
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
        case VM_INSTRUCTION_ALLOC:
            printf("%s", "ALLOC");
            break;
        case VM_INSTRUCTION_ALLOCC:
            printf("%s", "ALLOCC");
            break;
        case VM_INSTRUCTION_MULTCSD:
            printf("%s", "MULTCSD");
            break;
        case VM_INSTRUCTION_LDD:
            printf("%s", "LDD");
            break;
        case VM_INSTRUCTION_STD:
            printf("%s", "STD");
            break;
        case VM_INSTRUCTION_LDCD:
            printf("%s", "LDCD");
            break;
        case VM_INSTRUCTION_STCD:
            printf("%s", "STCD");
            break;
        case VM_INSTRUCTION_PUSHM:
            printf("%s", "PUSHM");
            break;
        case VM_INSTRUCTION_PUSHCM:
            printf("%s", "PUSHCM");
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
    for (uint8_t *iter = vm->interpreted; iter < vm->interpreted_cur; ++iter) {
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
        case VM_INSTRUCTION_PUSHID:
            printf("%s 0x%016lX", "PUSHID", *(uintptr_t*)(iter + 1));
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
        case VM_INSTRUCTION_DEF:
            printf("%s 0x%016lX", "DEF", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_DEFA:
            printf("%s 0x%016lX", "DEFA", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_ALLOC:
            printf("%s", "ALLOC");
            break;
        case VM_INSTRUCTION_ALLOCC:
            printf("%s", "ALLOCC");
            break;
        case VM_INSTRUCTION_MULTCSD:
            printf("%s", "MULTCSD");
            break;
        case VM_INSTRUCTION_LDD:
            printf("%s", "LDD");
            break;
        case VM_INSTRUCTION_STD:
            printf("%s", "STD");
            break;
        case VM_INSTRUCTION_LDCD:
            printf("%s", "LDCD");
            break;
        case VM_INSTRUCTION_STCD:
            printf("%s", "STCD");
            break;
        case VM_INSTRUCTION_PUSHM:
            printf("%s", "PUSHM");
            break;
        case VM_INSTRUCTION_PUSHCM:
            printf("%s", "PUSHCM");
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

void vm_reset(ForthVM *vm) {
    vm->interpreted_cur = vm->interpreted;
    vm->ip = vm->interpreted;
}

ForthVMStatus vm_run(ForthVM *vm, bool debug) {
    ForthVMStatus ret = VM_STATUS_OK;
    vm->state = VM_STATE_RUNNING;
    for (; vm->state != VM_STATE_HALTED; ++vm->ip) {
        if (debug) {
            printf("\nExecuting 0x%016lX ", (uintptr_t)vm->ip);
            switch (*vm->ip) {
            case VM_INSTRUCTION_HALT:
                printf("%s", "HALT");
                break;
            case VM_INSTRUCTION_RET:
                printf("%s", "RET");
                break;
            case VM_INSTRUCTION_PUSHD:
                printf("%s 0x%016lX", "PUSHD", *(uintptr_t*)(vm->ip + 1));
                break;
            case VM_INSTRUCTION_PUSHID:
                printf("%s 0x%016lX", "PUSHID", *(uintptr_t*)(vm->ip + 1));
                break;
            case VM_INSTRUCTION_PINT:
                printf("%s", "PINT");
                break;
            case VM_INSTRUCTION_EMIT:
                printf("%s", "EMIT");
                break;
            case VM_INSTRUCTION_CALL:
                printf("%s 0x%016lX", "CALL", *(uintptr_t*)(vm->ip + 1));
                break;
            case VM_INSTRUCTION_JZD:
                printf("%s 0x%016lX", "JZD", *(uintptr_t*)(vm->ip + 1));
                break;
            case VM_INSTRUCTION_JNZD:
                printf("%s 0x%016lX", "JNZD", *(uintptr_t*)(vm->ip + 1));
                break;
            case VM_INSTRUCTION_JMP:
                printf("%s 0x%016lX", "JMP", *(uintptr_t*)(vm->ip + 1));
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
                printf("%s 0x%016lX", "JNEC", *(uintptr_t*)(vm->ip + 1));
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
            case VM_INSTRUCTION_DEF:
                printf("%s 0x%016lX", "DEF", *(uintptr_t*)(vm->ip + 1));
                break;
            case VM_INSTRUCTION_DEFA:
                printf("%s 0x%016lX", "DEFA", *(uintptr_t*)(vm->ip + 1));
                break;
            case VM_INSTRUCTION_ALLOC:
                printf("%s", "ALLOC");
                break;
            case VM_INSTRUCTION_ALLOCC:
                printf("%s", "ALLOCC");
                break;
            case VM_INSTRUCTION_MULTCSD:
                printf("%s", "MULTCSD");
                break;
            case VM_INSTRUCTION_LDD:
                printf("%s", "LDD");
                break;
            case VM_INSTRUCTION_STD:
                printf("%s", "STD");
                break;
            case VM_INSTRUCTION_LDCD:
                printf("%s", "LDCD");
                break;
            case VM_INSTRUCTION_STCD:
                printf("%s", "STCD");
                break;
            case VM_INSTRUCTION_PUSHM:
                printf("%s", "PUSHM");
                break;
            case VM_INSTRUCTION_PUSHCM:
                printf("%s", "PUSHCM");
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
        switch (*vm->ip) {
        case VM_INSTRUCTION_HALT:
            vm->state = VM_STATE_HALTED;
            break;
        case VM_INSTRUCTION_RET:
            ret = vm_handler_ret(vm);
            break;
        case VM_INSTRUCTION_PUSHD:
            ret = vm_handler_pushd(vm);
            break;
        case VM_INSTRUCTION_PUSHID:
            ret = vm_handler_pushid(vm);
            break;
        case VM_INSTRUCTION_PINT:
            ret = vm_handler_pint(vm);
            break;
        case VM_INSTRUCTION_EMIT:
            ret = vm_handler_emit(vm);
            break;
        case VM_INSTRUCTION_CALL:
            ret = vm_handler_call(vm);
            break;
        case VM_INSTRUCTION_JZD:
            ret = vm_handler_jzd(vm);
            break;
        case VM_INSTRUCTION_JNZD:
            ret = vm_handler_jnzd(vm);
            break;
        case VM_INSTRUCTION_JMP:
            ret = vm_handler_jmp(vm);
            break;
        case VM_INSTRUCTION_2PUSHC:
            ret = vm_handler_2pushc(vm);
            break;
        case VM_INSTRUCTION_PEEKC:
            ret = vm_handler_peekc(vm);
            break;
        case VM_INSTRUCTION_2RMC:
            ret = vm_handler_2rmc(vm);
            break;
        case VM_INSTRUCTION_JNEC:
            ret = vm_handler_jnec(vm);
            break;
        case VM_INSTRUCTION_INCC:
            ret = vm_handler_incc(vm);
            break;
        case VM_INSTRUCTION_ADDC:
            ret = vm_handler_addc(vm);
            break;
        case VM_INSTRUCTION_3PEEKC:
            ret = vm_handler_3peekc(vm);
            break;
        case VM_INSTRUCTION_DEF:
            ret = vm_handler_def(vm);
            break;
        case VM_INSTRUCTION_DEFA:
            ret = vm_handler_defa(vm);
            break;
        case VM_INSTRUCTION_ALLOC:
            ret = vm_handler_alloc(vm);
            break;
        case VM_INSTRUCTION_ALLOCC:
            ret = vm_handler_allocc(vm);
            break;
        case VM_INSTRUCTION_MULTCSD:
            ret = vm_handler_multcsd(vm);
            break;
        case VM_INSTRUCTION_LDD:
            ret = vm_handler_ldd(vm);
            break;
        case VM_INSTRUCTION_STD:
            ret = vm_handler_std(vm);
            break;
        case VM_INSTRUCTION_LDCD:
            ret = vm_handler_ldcd(vm);
            break;
        case VM_INSTRUCTION_STCD:
            ret = vm_handler_stcd(vm);
            break;
        case VM_INSTRUCTION_PUSHM:
            ret = vm_handler_pushm(vm);
            break;
        case VM_INSTRUCTION_PUSHCM:
            ret = vm_handler_pushcm(vm);
            break;
        case VM_INSTRUCTION_ADDD:
            ret = vm_handler_addd(vm);
            break;
        case VM_INSTRUCTION_SUBD:
            ret = vm_handler_subd(vm);
            break;
        case VM_INSTRUCTION_MULTD:
            ret = vm_handler_multd(vm);
            break;
        case VM_INSTRUCTION_DIVD:
            ret = vm_handler_divd(vm);
            break;
        case VM_INSTRUCTION_INCD:
            ret = vm_handler_incd(vm);
            break;
        case VM_INSTRUCTION_MODD:
            ret = vm_handler_modd(vm);
            break;
        case VM_INSTRUCTION_DECD:
            ret = vm_handler_decd(vm);
            break;
        case VM_INSTRUCTION_DUBD:
            ret = vm_handler_dubd(vm);
            break;
        case VM_INSTRUCTION_HALFD:
            ret = vm_handler_halfd(vm);
            break;
        case VM_INSTRUCTION_LSLD:
            ret = vm_handler_lsld(vm);
            break;
        case VM_INSTRUCTION_LSRD:
            ret = vm_handler_lsrd(vm);
            break;
        case VM_INSTRUCTION_ANDD:
            ret = vm_handler_andd(vm);
            break;
        case VM_INSTRUCTION_ORD:
            ret = vm_handler_ord(vm);
            break;
        case VM_INSTRUCTION_XORD:
            ret = vm_handler_xord(vm);
            break;
        case VM_INSTRUCTION_NOTD:
            ret = vm_handler_notd(vm);
            break;
        case VM_INSTRUCTION_NEGD:
            ret = vm_handler_negd(vm);
            break;
        case VM_INSTRUCTION_GTD:
            ret = vm_handler_gtd(vm);
            break;
        case VM_INSTRUCTION_LTD:
            ret = vm_handler_ltd(vm);
            break;
        case VM_INSTRUCTION_EQD:
            ret = vm_handler_eqd(vm);
            break;
        case VM_INSTRUCTION_ULTD:
            ret = vm_handler_ultd(vm);
            break;
        case VM_INSTRUCTION_LTZD:
            ret = vm_handler_ltzd(vm);
            break;
        case VM_INSTRUCTION_EQZD:
            ret = vm_handler_eqzd(vm);
            break;
        case VM_INSTRUCTION_DUPD:
            ret = vm_handler_dupd(vm);
            break;
        case VM_INSTRUCTION_DROPD:
            ret = vm_handler_dropd(vm);
            break;
        case VM_INSTRUCTION_SWAPD:
            ret = vm_handler_swapd(vm);
            break;
        }
    }
    return ret;
}
