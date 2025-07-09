#ifndef VM_EXT_H_
#define VM_EXT_H_

#include <fsi/vm/vm.h>

typedef int (*Ext)(ForthVM*);
int vm_ext_fabs(ForthVM *vm);
int vm_ext_floor(ForthVM *vm);
int vm_ext_fround(ForthVM *vm);
int vm_ext_f_proximate(ForthVM *vm);
int vm_ext_f_power(ForthVM *vm);
int vm_ext_fsqrt(ForthVM *vm);
int vm_ext_fexp(ForthVM *vm);
int vm_ext_fexpm1(ForthVM *vm);
int vm_ext_fln(ForthVM *vm);
int vm_ext_flnp1(ForthVM *vm);
int vm_ext_flog(ForthVM *vm);
int vm_ext_fsin(ForthVM *vm);
int vm_ext_fcos(ForthVM *vm);
int vm_ext_ftan(ForthVM *vm);
int vm_ext_fsinh(ForthVM *vm);
int vm_ext_fcosh(ForthVM *vm);
int vm_ext_ftanh(ForthVM *vm);
int vm_ext_fasin(ForthVM *vm);
int vm_ext_facos(ForthVM *vm);
int vm_ext_fatan(ForthVM *vm);

#endif
