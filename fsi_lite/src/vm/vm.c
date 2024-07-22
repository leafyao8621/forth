#include "vm.h"

static uint8_t mem[65536];

uint8_t *vm_lookup;
uint8_t *vm_lookup_cur;
uint8_t *vm_lookup_end;
uint8_t *vm_lookup_addr;
uint8_t *vm_lookup_addr_cur;
uint8_t *vm_lookup_addr_end;
uint8_t *vm_lookup_meta;
uint8_t *vm_lookup_meta_cur;
uint8_t *vm_lookup_meta_end;
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
    vm_lookup = mem;
    vm_lookup_cur = vm_lookup;
    vm_lookup_end = mem + 5000;
    vm_lookup_addr = vm_lookup_end;
    vm_lookup_cur = vm_lookup_addr;
    vm_lookup_addr_end = vm_lookup_addr + sizeof(uint8_t*) * 500;
    vm_lookup_meta = vm_lookup_addr_end;
    vm_lookup_meta_cur = vm_lookup_meta;
    vm_lookup_meta_end = vm_lookup_meta + 500;
    vm_literal = vm_lookup_meta_end;
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
}
