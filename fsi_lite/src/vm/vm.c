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
uint8_t *vm_compiled;
uint8_t *vm_compiled_cur;
uint8_t *vm_compiled_end;
uint8_t *vm_interpreted;
uint8_t *vm_interpreted_cur;
uint8_t *vm_interpreted_end;
uint8_t *vm_memory;
uint8_t *vm_memory_cur;
uint8_t *vm_memory_end;

void vm_initialize() {
    vm_lookup = mem;
    vm_lookup_cur = vm_lookup;
    vm_lookup_end = mem + 2000;
    vm_lookup_addr = vm_lookup_end;
    vm_lookup_cur = vm_lookup_addr;
    vm_lookup_addr_end = vm_lookup_addr + sizeof(uint8_t*) * 200;
    vm_lookup_meta = vm_lookup_addr_end;
    vm_lookup_meta_cur = vm_lookup_meta;
    vm_lookup_meta_end = vm_lookup_meta + 200;
}
