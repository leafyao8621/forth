#include <stdio.h>
#include <stdlib.h>

#include <fsi/vm/vm.h>
#include <fsi/util/status.h>

#include "handler/handler.h"
#include "ext/ext.h"


#define BUILTIN_SIZE 117
#define BUILTIN_CALLEXT_SIZE 5
#define MEMORY_SIZE 2

ForthVMStatus vm_initialize(
    ForthVM *vm,
    size_t memory,
    size_t lookup,
    size_t literal,
    size_t ext,
    size_t interpreted,
    size_t data_stack,
    size_t float_stack,
    size_t return_stack,
    size_t compiled) {
    static const char *builtin[BUILTIN_SIZE] =
        {
            ".",
            "emit",
            "cr",
            "bl",
            "space",
            "spaces",
            ":",
            ";",
            "if",
            "then",
            "else",
            "case",
            "of",
            "endof",
            "endcase",
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
            "fill",
            "cells",
            "does>",
            "@",
            "!",
            "c@",
            "c!",
            "2@",
            "2!",
            "+!",
            ",",
            "c,",
            "+",
            "-",
            "*",
            "/",
            "mod",
            "1+",
            "cell+",
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
            "min",
            "max",
            "dup",
            "drop",
            "swap",
            "over",
            "nip",
            "2dup",
            "2drop",
            "2swap",
            "2over",
            "?dup",
            "rot",
            "pick",
            "depth",
            ">r",
            "r>",
            "2>r",
            "2r>",
            "char",
            "char+",
            "chars",
            "type",
            "s\"",
            ".\"",
            "c\"",
            "count",
            "d>f",
            "f>d",
            "f.",
            "f@",
            "f!",
            "floats",
            "float+",
            "f+",
            "f-",
            "f*",
            "f/",
            "fnegate",
            "f0<",
            "f0=",
            "f<",
            "fmin",
            "fmax",
            "fdup",
            "fdrop",
            "fswap",
            "fover",
            "frot",
            "fdepth",
            "fvariable"
        };
    static const char *builtin_callext_symbol[BUILTIN_CALLEXT_SIZE] =
        {
            "fabs",
            "floor",
            "fround",
            "f~",
            "f**"
        };
    static const Ext builtin_callext_value[BUILTIN_CALLEXT_SIZE] =
        {
            vm_ext_fabs,
            vm_ext_floor,
            vm_ext_fround,
            vm_ext_f_proximate,
            vm_ext_f_power
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
    vm->ext = vm->literal_end;
    vm->ext_cur = vm->ext;
    vm->ext_end = vm->ext + ext;
    vm->interpreted = vm->ext_end;
    vm->interpreted_cur = vm->interpreted;
    vm->interpreted_end = vm->interpreted + interpreted;
    vm->data_stack = vm->interpreted_end;
    vm->data_stack_cur = vm->data_stack;
    vm->data_stack_end = vm->data_stack + data_stack;
    vm->float_stack = vm->data_stack_end;
    vm->float_stack_cur = vm->float_stack;
    vm->float_stack_end = vm->float_stack + float_stack;
    vm->return_stack = vm->float_stack_end;
    vm->return_stack_cur = vm->return_stack;
    vm->return_stack_end = vm->return_stack + return_stack;
    vm->compiled = vm->return_stack_end;
    vm->compiled_cur = vm->compiled;
    vm->compiled_end = vm->compiled + compiled;
    vm->memory = vm->compiled_end;
    vm->memory_cur = vm->memory + sizeof(size_t);
    *(uint8_t**)vm->memory_cur = vm->memory;
    vm->memory_end = vm->mem + memory;
    const char **iter_builtin = builtin;
    const char *iter_str = 0;
    const char **iter_builtin_callext_symbol = builtin_callext_symbol;
    const Ext *iter_builtin_callext_value = builtin_callext_value;
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
        i < BUILTIN_CALLEXT_SIZE;
        ++i, ++iter_builtin_callext_symbol, ++iter_builtin_callext_value) {
        *(vm->lookup_cur++) = VM_LOOKUP_META_BUILTIN | VM_LOOKUP_META_CALLEXT;
        *(Ext*)(vm->lookup_cur) = *iter_builtin_callext_value;
        vm->lookup_cur += sizeof(size_t);
        for (
            iter_str = *iter_builtin_callext_symbol;
            *iter_str;
            ++iter_str, ++vm->lookup_cur) {
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
        *(uint8_t**)vm->memory_cur += sizeof(size_t);
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
        if (*iter & VM_LOOKUP_META_MEMORY) {
            printf("%s ", "MEMORY");
        }
        if (*iter & VM_LOOKUP_META_CALL) {
            printf("%s ", "CALL");
        }
        if (*iter & VM_LOOKUP_META_CREATE) {
            printf("%s ", "CREATE");
        }
        if (*iter & VM_LOOKUP_META_DOES) {
            printf("%s ", "DOES");
        }
        if (*iter & VM_LOOKUP_META_INDIRECT) {
            printf("%s ", "INDIRECT");
        }
        if (*iter & VM_LOOKUP_META_CALLEXT) {
            printf("%s ", "CALLEXT");
        }
        printf("0x%016lX ", *(uintptr_t*)(++iter));
        iter += sizeof(uintptr_t);
        if (iter[-1 - sizeof(uintptr_t)] & VM_LOOKUP_META_INDIRECT) {
            printf("0x%016lX ", *(uintptr_t*)iter);
            iter += sizeof(uintptr_t);
        }
        for (; *iter; ++iter) {
            putchar(*iter);
        }
        putchar(10);
    }
    puts("Literal:");
    for (uint8_t *iter = vm->literal; iter < vm->literal_cur;) {
        printf("0x%016lX ", (uintptr_t)iter);
        for (uint8_t i = 0; iter < vm->literal_cur && i < 8; ++iter, ++i) {
            printf(
                "%02X(%c)", *iter, *iter >= 32 && *iter <= 126 ? *iter : '*');
        }
        putchar('\n');
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
        case VM_INSTRUCTION_JNED:
            printf("%s 0x%016lX", "JNED", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_2PUSHR:
            printf("%s", "2PUSHR");
            break;
        case VM_INSTRUCTION_PEEKR:
            printf("%s", "PEEKR");
            break;
        case VM_INSTRUCTION_2RMR:
            printf("%s", "2RMR");
            break;
        case VM_INSTRUCTION_JNER:
            printf("%s 0x%016lX", "JNER", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_INCR:
            printf("%s", "INCR");
            break;
        case VM_INSTRUCTION_ADDR:
            printf("%s", "ADDR");
            break;
        case VM_INSTRUCTION_3PEEKR:
            printf("%s", "3PEEKR");
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
        case VM_INSTRUCTION_2LDD:
            printf("%s", "2LDD");
            break;
        case VM_INSTRUCTION_2STD:
            printf("%s", "2STD");
            break;
        case VM_INSTRUCTION_INCMD:
            printf("%s", "INCMD");
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
        case VM_INSTRUCTION_INCCD:
            printf("%s", "INCCD");
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
        case VM_INSTRUCTION_MIND:
            printf("%s", "MIND");
            break;
        case VM_INSTRUCTION_MAXD:
            printf("%s", "MAXD");
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
        case VM_INSTRUCTION_OVERD:
            printf("%s", "OVERD");
            break;
        case VM_INSTRUCTION_NIPD:
            printf("%s", "NIPD");
            break;
        case VM_INSTRUCTION_2DUPD:
            printf("%s", "2DUPD");
            break;
        case VM_INSTRUCTION_2DROPD:
            printf("%s", "2DROPD");
            break;
        case VM_INSTRUCTION_2SWAPD:
            printf("%s", "2SWAPD");
            break;
        case VM_INSTRUCTION_2OVERD:
            printf("%s", "2OVERD");
            break;
        case VM_INSTRUCTION_DUPNZD:
            printf("%s", "DUPNZD");
            break;
        case VM_INSTRUCTION_ROTD:
            printf("%s", "ROTD");
            break;
        case VM_INSTRUCTION_PICKD:
            printf("%s", "PICKD");
            break;
        case VM_INSTRUCTION_PUSHDD:
            printf("%s", "PUSHDD");
            break;
        case VM_INSTRUCTION_PUSHR:
            printf("%s", "PUSHR");
            break;
        case VM_INSTRUCTION_POPR:
            printf("%s", "POPR");
            break;
        case VM_INSTRUCTION_2POPR:
            printf("%s", "2POPR");
            break;
        case VM_INSTRUCTION_PUSHF:
            printf("%s", "PUSHF");
            break;
        case VM_INSTRUCTION_POPF:
            printf("%s", "POPF");
            break;
        case VM_INSTRUCTION_PFLOAT:
            printf("%s", "PFLOAT");
            break;
        case VM_INSTRUCTION_LDF:
            printf("%s", "LDF");
            break;
        case VM_INSTRUCTION_STF:
            printf("%s", "STF");
            break;
        case VM_INSTRUCTION_MULTFSD:
            printf("%s", "MULTFSD");
            break;
        case VM_INSTRUCTION_INCFD:
            printf("%s", "INCFD");
            break;
        case VM_INSTRUCTION_CALLEXT:
            printf("%s 0x%016lX", "CALLEXT", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_ADDF:
            printf("%s", "ADDF");
            break;
        case VM_INSTRUCTION_SUBF:
            printf("%s", "SUBF");
            break;
        case VM_INSTRUCTION_MULTF:
            printf("%s", "MULTF");
            break;
        case VM_INSTRUCTION_DIVF:
            printf("%s", "DIVF");
            break;
        case VM_INSTRUCTION_NEGF:
            printf("%s", "NEGF");
            break;
        case VM_INSTRUCTION_LTZF:
            printf("%s", "LTZF");
            break;
        case VM_INSTRUCTION_EQZF:
            printf("%s", "EQZF");
            break;
        case VM_INSTRUCTION_LTF:
            printf("%s", "LTF");
            break;
        case VM_INSTRUCTION_MINF:
            printf("%s", "MINF");
            break;
        case VM_INSTRUCTION_MAXF:
            printf("%s", "MAXF");
            break;
        case VM_INSTRUCTION_DUPF:
            printf("%s", "DUPF");
            break;
        case VM_INSTRUCTION_DROPF:
            printf("%s", "DROPF");
            break;
        case VM_INSTRUCTION_SWAPF:
            printf("%s", "SWAPF");
            break;
        case VM_INSTRUCTION_OVERF:
            printf("%s", "OVERF");
            break;
        case VM_INSTRUCTION_ROTF:
            printf("%s", "ROTF");
            break;
        case VM_INSTRUCTION_PUSHDFD:
            printf("%s", "PUSHDFD");
            break;
        case VM_INSTRUCTION_DEFAF:
            printf("%s", "DEFAF");
            break;
        case VM_INSTRUCTION_ALLOCF:
            printf("%s", "ALLOCF");
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
        case VM_INSTRUCTION_2PUSHR:
            printf("%s", "2PUSHR");
            break;
        case VM_INSTRUCTION_2RMR:
            printf("%s", "2RMR");
            break;
        case VM_INSTRUCTION_JNER:
            printf("%s 0x%016lX", "JNER", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_INCR:
            printf("%s", "INCR");
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
        case VM_INSTRUCTION_2LDD:
            printf("%s", "2LDD");
            break;
        case VM_INSTRUCTION_2STD:
            printf("%s", "2STD");
            break;
        case VM_INSTRUCTION_INCMD:
            printf("%s", "INCMD");
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
        case VM_INSTRUCTION_INCCD:
            printf("%s", "INCCD");
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
        case VM_INSTRUCTION_MIND:
            printf("%s", "MIND");
            break;
        case VM_INSTRUCTION_MAXD:
            printf("%s", "MAXD");
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
        case VM_INSTRUCTION_OVERD:
            printf("%s", "OVERD");
            break;
        case VM_INSTRUCTION_NIPD:
            printf("%s", "NIPD");
            break;
        case VM_INSTRUCTION_2DUPD:
            printf("%s", "2DUPD");
            break;
        case VM_INSTRUCTION_2DROPD:
            printf("%s", "2DROPD");
            break;
        case VM_INSTRUCTION_2SWAPD:
            printf("%s", "2SWAPD");
            break;
        case VM_INSTRUCTION_2OVERD:
            printf("%s", "2OVERD");
            break;
        case VM_INSTRUCTION_DUPNZD:
            printf("%s", "DUPNZD");
            break;
        case VM_INSTRUCTION_ROTD:
            printf("%s", "ROTD");
            break;
        case VM_INSTRUCTION_PICKD:
            printf("%s", "PICKD");
            break;
        case VM_INSTRUCTION_PUSHDD:
            printf("%s", "PUSHDD");
            break;
        case VM_INSTRUCTION_PUSHR:
            printf("%s", "PUSHR");
            break;
        case VM_INSTRUCTION_POPR:
            printf("%s", "POPR");
            break;
        case VM_INSTRUCTION_2POPR:
            printf("%s", "2POPR");
            break;
        case VM_INSTRUCTION_PUSHF:
            printf("%s", "PUSHF");
            break;
        case VM_INSTRUCTION_POPF:
            printf("%s", "POPF");
            break;
        case VM_INSTRUCTION_PFLOAT:
            printf("%s", "PFLOAT");
            break;
        case VM_INSTRUCTION_LDF:
            printf("%s", "LDF");
            break;
        case VM_INSTRUCTION_STF:
            printf("%s", "STF");
            break;
        case VM_INSTRUCTION_MULTFSD:
            printf("%s", "MULTFSD");
            break;
        case VM_INSTRUCTION_INCFD:
            printf("%s", "INCFD");
            break;
        case VM_INSTRUCTION_CALLEXT:
            printf("%s 0x%016lX", "CALLEXT", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_ADDF:
            printf("%s", "ADDF");
            break;
        case VM_INSTRUCTION_SUBF:
            printf("%s", "SUBF");
            break;
        case VM_INSTRUCTION_MULTF:
            printf("%s", "MULTF");
            break;
        case VM_INSTRUCTION_DIVF:
            printf("%s", "DIVF");
            break;
        case VM_INSTRUCTION_NEGF:
            printf("%s", "NEGF");
            break;
        case VM_INSTRUCTION_LTZF:
            printf("%s", "LTZF");
            break;
        case VM_INSTRUCTION_EQZF:
            printf("%s", "EQZF");
            break;
        case VM_INSTRUCTION_LTF:
            printf("%s", "LTF");
            break;
        case VM_INSTRUCTION_MINF:
            printf("%s", "MINF");
            break;
        case VM_INSTRUCTION_MAXF:
            printf("%s", "MAXF");
            break;
        case VM_INSTRUCTION_DUPF:
            printf("%s", "DUPF");
            break;
        case VM_INSTRUCTION_DROPF:
            printf("%s", "DROPF");
            break;
        case VM_INSTRUCTION_SWAPF:
            printf("%s", "SWAPF");
            break;
        case VM_INSTRUCTION_OVERF:
            printf("%s", "OVERF");
            break;
        case VM_INSTRUCTION_ROTF:
            printf("%s", "ROTF");
            break;
        case VM_INSTRUCTION_PUSHDFD:
            printf("%s", "PUSHDFD");
            break;
        case VM_INSTRUCTION_DEFAF:
            printf("%s", "DEFAF");
            break;
        case VM_INSTRUCTION_ALLOCF:
            printf("%s", "ALLOCF");
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
            case VM_INSTRUCTION_JNED:
                printf("%s 0x%016lX", "JNED", *(uintptr_t*)(vm->ip + 1));
                break;
            case VM_INSTRUCTION_2PUSHR:
                printf("%s", "2PUSHR");
                break;
            case VM_INSTRUCTION_PEEKR:
                printf("%s", "PEEKR");
                break;
            case VM_INSTRUCTION_2RMR:
                printf("%s", "2RMR");
                break;
            case VM_INSTRUCTION_JNER:
                printf("%s 0x%016lX", "JNER", *(uintptr_t*)(vm->ip + 1));
                break;
            case VM_INSTRUCTION_INCR:
                printf("%s", "INCR");
                break;
            case VM_INSTRUCTION_ADDR:
                printf("%s", "ADDR");
                break;
            case VM_INSTRUCTION_3PEEKR:
                printf("%s", "3PEEKR");
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
            case VM_INSTRUCTION_2LDD:
                printf("%s", "2LDD");
                break;
            case VM_INSTRUCTION_2STD:
                printf("%s", "2STD");
                break;
            case VM_INSTRUCTION_INCMD:
                printf("%s", "INCMD");
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
            case VM_INSTRUCTION_INCCD:
                printf("%s", "INCCD");
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
            case VM_INSTRUCTION_MIND:
                printf("%s", "MIND");
                break;
            case VM_INSTRUCTION_MAXD:
                printf("%s", "MAXD");
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
            case VM_INSTRUCTION_OVERD:
                printf("%s", "OVERD");
                break;
            case VM_INSTRUCTION_NIPD:
                printf("%s", "NIPD");
                break;
            case VM_INSTRUCTION_2DUPD:
                printf("%s", "2DUPD");
                break;
            case VM_INSTRUCTION_2DROPD:
                printf("%s", "2DROPD");
                break;
            case VM_INSTRUCTION_2SWAPD:
                printf("%s", "2SWAPD");
                break;
            case VM_INSTRUCTION_2OVERD:
                printf("%s", "2OVERD");
                break;
            case VM_INSTRUCTION_DUPNZD:
                printf("%s", "DUPNZD");
                break;
            case VM_INSTRUCTION_ROTD:
                printf("%s", "ROTD");
                break;
            case VM_INSTRUCTION_PICKD:
                printf("%s", "PICKD");
                break;
            case VM_INSTRUCTION_PUSHDD:
                printf("%s", "PUSHDD");
                break;
            case VM_INSTRUCTION_PUSHR:
                printf("%s", "PUSHR");
                break;
            case VM_INSTRUCTION_POPR:
                printf("%s", "POPR");
                break;
            case VM_INSTRUCTION_2POPR:
                printf("%s", "2POPR");
                break;
            case VM_INSTRUCTION_PUSHF:
                printf("%s", "PUSHF");
                break;
            case VM_INSTRUCTION_POPF:
                printf("%s", "POPF");
                break;
            case VM_INSTRUCTION_PFLOAT:
                printf("%s", "PFLOAT");
                break;
            case VM_INSTRUCTION_LDF:
                printf("%s", "LDF");
                break;
            case VM_INSTRUCTION_STF:
                printf("%s", "STF");
                break;
            case VM_INSTRUCTION_MULTFSD:
                printf("%s", "MULTFSD");
                break;
            case VM_INSTRUCTION_INCFD:
                printf("%s", "INCFD");
                break;
            case VM_INSTRUCTION_CALLEXT:
                printf("%s 0x%016lX", "CALLEXT", *(uintptr_t*)(vm->ip + 1));
                break;
            case VM_INSTRUCTION_ADDF:
                printf("%s", "ADDF");
                break;
            case VM_INSTRUCTION_SUBF:
                printf("%s", "SUBF");
                break;
            case VM_INSTRUCTION_MULTF:
                printf("%s", "MULTF");
                break;
            case VM_INSTRUCTION_DIVF:
                printf("%s", "DIVF");
                break;
            case VM_INSTRUCTION_NEGF:
                printf("%s", "NEGF");
                break;
            case VM_INSTRUCTION_LTZF:
                printf("%s", "LTZF");
                break;
            case VM_INSTRUCTION_EQZF:
                printf("%s", "EQZF");
                break;
            case VM_INSTRUCTION_LTF:
                printf("%s", "LTF");
                break;
            case VM_INSTRUCTION_MINF:
                printf("%s", "MINF");
                break;
            case VM_INSTRUCTION_MAXF:
                printf("%s", "MAXF");
                break;
            case VM_INSTRUCTION_DUPF:
                printf("%s", "DUPF");
                break;
            case VM_INSTRUCTION_DROPF:
                printf("%s", "DROPF");
                break;
            case VM_INSTRUCTION_SWAPF:
                printf("%s", "SWAPF");
                break;
            case VM_INSTRUCTION_OVERF:
                printf("%s", "OVERF");
                break;
            case VM_INSTRUCTION_ROTF:
                printf("%s", "ROTF");
                break;
            case VM_INSTRUCTION_PUSHDFD:
                printf("%s", "PUSHDFD");
                break;
            case VM_INSTRUCTION_DEFAF:
                printf("%s", "DEFAF");
                break;
            case VM_INSTRUCTION_ALLOCF:
                printf("%s", "ALLOCF");
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
        case VM_INSTRUCTION_JNED:
            ret = vm_handler_jned(vm);
            break;
        case VM_INSTRUCTION_2PUSHR:
            ret = vm_handler_2pushr(vm);
            break;
        case VM_INSTRUCTION_PEEKR:
            ret = vm_handler_peekr(vm);
            break;
        case VM_INSTRUCTION_2RMR:
            ret = vm_handler_2rmr(vm);
            break;
        case VM_INSTRUCTION_JNER:
            ret = vm_handler_jner(vm);
            break;
        case VM_INSTRUCTION_INCR:
            ret = vm_handler_incr(vm);
            break;
        case VM_INSTRUCTION_ADDR:
            ret = vm_handler_addr(vm);
            break;
        case VM_INSTRUCTION_3PEEKR:
            ret = vm_handler_3peekr(vm);
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
        case VM_INSTRUCTION_2LDD:
            ret = vm_handler_2ldd(vm);
            break;
        case VM_INSTRUCTION_2STD:
            ret = vm_handler_2std(vm);
            break;
        case VM_INSTRUCTION_INCMD:
            ret = vm_handler_incmd(vm);
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
        case VM_INSTRUCTION_MODD:
            ret = vm_handler_modd(vm);
            break;
        case VM_INSTRUCTION_INCD:
            ret = vm_handler_incd(vm);
            break;
        case VM_INSTRUCTION_INCCD:
            ret = vm_handler_inccd(vm);
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
        case VM_INSTRUCTION_MIND:
            ret = vm_handler_mind(vm);
            break;
        case VM_INSTRUCTION_MAXD:
            ret = vm_handler_maxd(vm);
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
        case VM_INSTRUCTION_OVERD:
            ret = vm_handler_overd(vm);
            break;
        case VM_INSTRUCTION_NIPD:
            ret = vm_handler_nipd(vm);
            break;
        case VM_INSTRUCTION_2DUPD:
            ret = vm_handler_2dupd(vm);
            break;
        case VM_INSTRUCTION_2DROPD:
            ret = vm_handler_2dropd(vm);
            break;
        case VM_INSTRUCTION_2SWAPD:
            ret = vm_handler_2swapd(vm);
            break;
        case VM_INSTRUCTION_2OVERD:
            ret = vm_handler_2overd(vm);
            break;
        case VM_INSTRUCTION_DUPNZD:
            ret = vm_handler_dupnzd(vm);
            break;
        case VM_INSTRUCTION_ROTD:
            ret = vm_handler_rotd(vm);
            break;
        case VM_INSTRUCTION_PICKD:
            ret = vm_handler_pickd(vm);
            break;
        case VM_INSTRUCTION_PUSHDD:
            ret = vm_handler_pushdd(vm);
            break;
        case VM_INSTRUCTION_PUSHR:
            ret = vm_handler_pushr(vm);
            break;
        case VM_INSTRUCTION_POPR:
            ret = vm_handler_popr(vm);
            break;
        case VM_INSTRUCTION_2POPR:
            ret = vm_handler_2popr(vm);
            break;
        case VM_INSTRUCTION_PUSHF:
            ret = vm_handler_pushf(vm);
            break;
        case VM_INSTRUCTION_POPF:
            ret = vm_handler_popf(vm);
            break;
        case VM_INSTRUCTION_PFLOAT:
            ret = vm_handler_pfloat(vm);
            break;
        case VM_INSTRUCTION_LDF:
            ret = vm_handler_ldf(vm);
            break;
        case VM_INSTRUCTION_STF:
            ret = vm_handler_stf(vm);
            break;
        case VM_INSTRUCTION_MULTFSD:
            ret = vm_handler_multfsd(vm);
            break;
        case VM_INSTRUCTION_INCFD:
            ret = vm_handler_incfd(vm);
            break;
        case VM_INSTRUCTION_CALLEXT:
            ret = vm_handler_callext(vm);
            break;
        case VM_INSTRUCTION_ADDF:
            ret = vm_handler_addf(vm);
            break;
        case VM_INSTRUCTION_SUBF:
            ret = vm_handler_subf(vm);
            break;
        case VM_INSTRUCTION_MULTF:
            ret = vm_handler_multf(vm);
            break;
        case VM_INSTRUCTION_DIVF:
            ret = vm_handler_divf(vm);
            break;
        case VM_INSTRUCTION_NEGF:
            ret = vm_handler_negf(vm);
            break;
        case VM_INSTRUCTION_LTZF:
            ret = vm_handler_ltzf(vm);
            break;
        case VM_INSTRUCTION_EQZF:
            ret = vm_handler_eqzf(vm);
            break;
        case VM_INSTRUCTION_LTF:
            ret = vm_handler_ltf(vm);
            break;
        case VM_INSTRUCTION_MINF:
            ret = vm_handler_minf(vm);
            break;
        case VM_INSTRUCTION_MAXF:
            ret = vm_handler_maxf(vm);
            break;
        case VM_INSTRUCTION_DUPF:
            ret = vm_handler_dupf(vm);
            break;
        case VM_INSTRUCTION_DROPF:
            ret = vm_handler_dropf(vm);
            break;
        case VM_INSTRUCTION_SWAPF:
            ret = vm_handler_swapf(vm);
            break;
        case VM_INSTRUCTION_OVERF:
            ret = vm_handler_overf(vm);
            break;
        case VM_INSTRUCTION_ROTF:
            ret = vm_handler_rotf(vm);
            break;
        case VM_INSTRUCTION_PUSHDFD:
            ret = vm_handler_pushdfd(vm);
            break;
        case VM_INSTRUCTION_DEFAF:
            ret = vm_handler_defaf(vm);
            break;
        case VM_INSTRUCTION_ALLOCF:
            ret = vm_handler_allocf(vm);
            break;
        }
    }
    return ret;
}
