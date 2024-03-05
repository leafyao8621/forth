#include <stdio.h>

#include <fsi/util/read_text.h>

int read_text(char *fn, String *str) {
    FILE *fin = fopen(fn, "rb");
    if (!fin) {
        return 1;
    }
    char buf[1000];
    size_t n_read = 0;
    int ret = 0;
    for (; (n_read = fread(buf, 1, 1000, fin)) == 1000;) {
        ret = DArrayChar_push_back_batch(str, buf, 1000);
        if (ret) {
            fclose(fin);
            return 2;
        }
    }
    ret = DArrayChar_push_back_batch(str, buf, n_read);
    if (ret) {
        fclose(fin);
        return 2;
    }
    char chr = 0;
    ret = DArrayChar_push_back(str, &chr);
    if (ret) {
        fclose(fin);
        return 2;
    }
    fclose(fin);
    return 0;
}
