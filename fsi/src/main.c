#include <stdio.h>
#include <fsi/fsi.h>

#define GETERR printf("%d %s\n", err, fsi_errcode_lookup[err])

int main(void) {
    ForthVM vm;
    ForthVMErr err;
    err = fsi_initialize(&vm);
    GETERR;
    fsi_finalize(&vm);
    return 0;
}
