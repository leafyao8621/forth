#include <stdio.h>

#include "util.h"

void generate(char *label, char *value) {
    printf("create %s\n    ", label);
    size_t cnt = 0;
    for (char *i = value; *i; ++i, ++cnt) {
        printf("%hhd c,", *i);
        if (cnt && !(cnt % 10)) {
            printf("\n    ");
        } else {
            putchar(' ');
        }
    }
    putchar(10);
}
