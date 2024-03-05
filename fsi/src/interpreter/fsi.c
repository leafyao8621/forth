#include <stdio.h>

#include <fsi/fsi.h>
#include <fsi/util/read_text.h>

ForthVMErr fsi_initialize(ForthVM *vm) {
    return ForthVM_initialize(vm);
}

void fsi_finalize(ForthVM *vm) {
    ForthVM_finalize(vm);
}

ForthVMErr fsi_load_file(ForthVM *vm, char *fn) {
    String buf;
    int ret = DArrayChar_initialize(&buf, 1001);
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    ret = read_text(fn, &buf);
    switch (ret) {
    case 2:
        return FORTHVM_ERR_OUT_OF_MEMORY;
    case 1:
        return FORTHVM_ERR_FILE_IO;
    }
    puts(buf.data);
    ForthVMErr err = ForthVM_load(vm, buf.data);
    DArrayChar_finalize(&buf);
    return err;
}
