#include <fsi/fsi.h>

ForthVMErr fsi_initialize(ForthVM *vm) {
    return ForthVM_initialize(vm);
}

void fsi_finalize(ForthVM *vm) {
    ForthVM_finalize(vm);
}
