#include <stdio.h>
#include <fsi/fsi.h>

#define GETERR printf("%d %s\n", err, fsi_errcode_lookup[err])

int main(void) {
    ForthVM vm;
    ForthVMErr err;
    err = fsi_initialize(&vm);
    GETERR;
    if (err) {
        return 0;
    }
    err = fsi_load_file(&vm, "script/a.fs");
    GETERR;
    if (err) {
        fsi_finalize(&vm);
        return 0;
    }
    fsi_log(&vm);
    err = fsi_execute(&vm);
    GETERR;
    fsi_finalize(&vm);
    return 0;
}
