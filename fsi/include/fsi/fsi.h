#ifndef FSI_H_
#define FSI_H_

#include <fsi/interpreter/vm.h>
#include <fsi/util/errcode.h>

ForthVMErr fsi_initialize(ForthVM *vm);
void fsi_finalize(ForthVM *vm);
ForthVMErr fsi_load_file(ForthVM *vm, char *fn);
ForthVMErr fsi_load_str(ForthVM *vm, char *str);
ForthVMErr fsi_execute(ForthVM *vm);
ForthVMErr fsi_execute_redirected(ForthVM *vm, String *in, String *out);

#endif
