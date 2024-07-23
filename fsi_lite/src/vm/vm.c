#include <stdio.h>
#include "vm.h"

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

void vm_initialize() {
    static const char *builtin[2] = {
        ".",
        "cr"
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
    vm_data_stack = vm_literal_end;
    vm_data_stack_cur = vm_data_stack;
    vm_data_stack_end = vm_data_stack + 2000;
    vm_control_stack = vm_literal_end;
    vm_control_stack_cur = vm_control_stack;
    vm_control_stack_end = vm_control_stack + 2000;
    vm_compiled = vm_control_stack_end;
    vm_compiled_cur = vm_compiled;
    vm_compiled_end = vm_compiled + 24000;
    vm_memory = vm_compiled_end;
    vm_memory_cur = vm_memory;
    vm_memory_end = mem + 65536;
    for (; i < 2; ++i, ++iter_builtin) {
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
        switch (*iter) {
        case VM_LOOKUP_META_BUILTIN:
            printf("%s ", "BUILTIN");
            break;
        }
        printf("%016lX ", *(size_t*)(++iter));
        iter += sizeof(size_t);
        for (; *iter; ++iter) {
            putchar(*iter);
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
            printf("%s %016lX", "PUSHD", *(uintptr_t*)(iter + 1));
            iter += sizeof(uintptr_t);
            break;
        case VM_INSTRUCTION_PINT:
            printf("%s", "PINT");
            break;
        case VM_INSTRUCTION_PCHR:
            printf("%s", "PCHR");
            break;
        }
        putchar(10);
    }
}

void vm_reset(void) {
    vm_interpreted_cur = vm_interpreted;
}
