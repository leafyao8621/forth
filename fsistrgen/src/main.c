#include <stdio.h>

#include "util.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        puts("Usage: fsistrgen <label> <string>");
        return 1;
    }
    generate(argv[1], argv[2]);
    return 0;
}
