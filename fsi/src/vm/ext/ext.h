#ifndef VM_EXT_H_
#define VM_EXT_H_

#include <fsi/vm/vm.h>

typedef int (*Ext)(ForthVM*);
int vm_ext_fabs(ForthVM *vm);
int vm_ext_floor(ForthVM *vm);
int vm_ext_fround(ForthVM *vm);
int vm_ext_f_proximate(ForthVM *vm);
int vm_ext_f_power(ForthVM *vm);

#endif
