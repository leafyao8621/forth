#ifndef VM_HANDLER_H_
#define VM_HANDLER_H_

#include <fsi/vm/vm.h>

int vm_handler_ret(ForthVM *vm);
int vm_handler_pushd(ForthVM *vm);
int vm_handler_pushid(ForthVM *vm);
int vm_handler_pint(ForthVM *vm);
int vm_handler_emit(ForthVM *vm);
int vm_handler_call(ForthVM *vm);
int vm_handler_jzd(ForthVM *vm);
int vm_handler_jnzd(ForthVM *vm);
int vm_handler_jmp(ForthVM *vm);
int vm_handler_2pushc(ForthVM *vm);
int vm_handler_peekc(ForthVM *vm);
int vm_handler_2rmc(ForthVM *vm);
int vm_handler_jnec(ForthVM *vm);
int vm_handler_incc(ForthVM *vm);
int vm_handler_addc(ForthVM *vm);
int vm_handler_3peekc(ForthVM *vm);
int vm_handler_def(ForthVM *vm);
int vm_handler_defa(ForthVM *vm);
int vm_handler_alloc(ForthVM *vm);
int vm_handler_multcsd(ForthVM *vm);
int vm_handler_ldd(ForthVM *vm);
int vm_handler_std(ForthVM *vm);
int vm_handler_ldcd(ForthVM *vm);
int vm_handler_stcd(ForthVM *vm);
int vm_handler_pushm(ForthVM *vm);
int vm_handler_pushcm(ForthVM *vm);
int vm_handler_addd(ForthVM *vm);
int vm_handler_subd(ForthVM *vm);
int vm_handler_multd(ForthVM *vm);
int vm_handler_divd(ForthVM *vm);
int vm_handler_modd(ForthVM *vm);
int vm_handler_incd(ForthVM *vm);
int vm_handler_decd(ForthVM *vm);
int vm_handler_dubd(ForthVM *vm);
int vm_handler_halfd(ForthVM *vm);
int vm_handler_lsld(ForthVM *vm);
int vm_handler_lsrd(ForthVM *vm);
int vm_handler_andd(ForthVM *vm);
int vm_handler_ord(ForthVM *vm);
int vm_handler_xord(ForthVM *vm);
int vm_handler_notd(ForthVM *vm);
int vm_handler_negd(ForthVM *vm);
int vm_handler_gtd(ForthVM *vm);
int vm_handler_ltd(ForthVM *vm);
int vm_handler_eqd(ForthVM *vm);
int vm_handler_ultd(ForthVM *vm);
int vm_handler_ltzd(ForthVM *vm);
int vm_handler_eqzd(ForthVM *vm);
int vm_handler_dupd(ForthVM *vm);
int vm_handler_dropd(ForthVM *vm);
int vm_handler_swapd(ForthVM *vm);

#endif
