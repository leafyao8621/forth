#include <stdio.h>
#include <fsi/fsi.h>

#define GETERR printf("%d %s\n", err, fsi_errcode_lookup[err])

int main(int argc, const char **argv) {
    ForthVM vm;
    ForthVMErr err;
    if (argc != 2) {
        puts("Usage: fsi <file name>");
        return 0;
    }
    err = fsi_initialize(&vm);
    GETERR;
    if (err) {
        return 0;
    }
    err = fsi_load_file(&vm, (char*)argv[1]);
    GETERR;
    if (err) {
        fsi_finalize(&vm);
        return 0;
    }
    // fsi_log(&vm);
    err = fsi_execute(&vm);
    GETERR;
    fsi_finalize(&vm);
    return 0;
}
