#ifndef VM_VM_H_
#define VM_VM_H_

#include <stdint.h>

extern uint8_t *vm_lookup;
extern uint8_t *vm_lookup_end;
extern uint8_t *vm_lookup_addr;
extern uint8_t *vm_lookup_addr_end;
extern uint8_t *vm_lookup_meta;
extern uint8_t *vm_lookup_meta_end;
extern uint8_t *vm_compiled;
extern uint8_t *vm_compiled_end;
extern uint8_t *vm_interpreted;
extern uint8_t *vm_interpreted_end;
extern uint8_t *vm_memory;
extern uint8_t *vm_memory_end;

void vm_initialize();

#endif
