#include <stdio.h>

#include <containers/dstring.h>

#include "compile.h"

ErrCompile compile(void) {
    FILE *fin = fopen("moddef.txt", "r");
    if (!fin) {
        return ERR_COMPILE_OPEN;
    }
    FILE *fout = fopen("moddef", "w");
    if (!fout) {
        fclose(fin);
        return ERR_COMPILE_OPEN;
    }
    String buf;
    if (DArrayChar_initialize(&buf, 80)) {
        fclose(fin);
        fclose(fout);
        return ERR_COMPILE_OUT_OF_MEMORY;
    }
    char zero = 0;
    CompileState state = COMPILE_STATE_MODE;
    uint8_t mode = 0;
    for (; !feof(fin);) {
        for (char in = fgetc(fin); in != '\n' && !feof(fin); in = fgetc(fin)) {
            if (DArrayChar_push_back(&buf, &in)) {
                fclose(fin);
                fclose(fout);
                DArrayChar_finalize(&buf);
                return ERR_COMPILE_OUT_OF_MEMORY;
            }
        }
        if (!buf.size) {
            break;
        }
        if (DArrayChar_push_back(&buf, &zero)) {
            fclose(fin);
            fclose(fout);
            DArrayChar_finalize(&buf);
            return ERR_COMPILE_OUT_OF_MEMORY;
        }
        switch (state) {
        case COMPILE_STATE_MODE:
            if (!strcmp(buf.data, "pr")) {
                mode = COMPILE_MODE_PARSE | COMPILE_MODE_RUN;
            } else if (!strcmp(buf.data, "p")) {
                mode = COMPILE_MODE_PARSE;
            } else if (!strcmp(buf.data, "r")) {
                mode = COMPILE_MODE_RUN;
            } else {
                fclose(fin);
                fclose(fout);
                DArrayChar_finalize(&buf);
                return ERR_COMPILE_INVALID_MODE;
            }
            fwrite(&mode, sizeof(uint8_t), 1, fout);
            state = COMPILE_STATE_NAME;
            break;
        case COMPILE_STATE_NAME:
            fwrite(&buf.size, sizeof(size_t), 1, fout);
            fwrite(buf.data, buf.size, 1, fout);
            if (mode & COMPILE_MODE_PARSE) {
                state = COMPILE_STATE_PARSE;
            } else if (mode & COMPILE_MODE_RUN) {
                state = COMPILE_STATE_RUN;
            }
            break;
        case COMPILE_STATE_PARSE:
            fwrite(&buf.size, sizeof(size_t), 1, fout);
            fwrite(buf.data, buf.size, 1, fout);
            if (mode & COMPILE_MODE_RUN) {
                state = COMPILE_STATE_RUN;
            } else {
                state = COMPILE_STATE_MODE;
            }
            break;
        case COMPILE_STATE_RUN:
            fwrite(&buf.size, sizeof(size_t), 1, fout);
            fwrite(buf.data, buf.size, 1, fout);
            state = COMPILE_STATE_MODE;
            break;
        }
        DArrayChar_clear(&buf);
    }
    fclose(fin);
    fclose(fout);
    DArrayChar_finalize(&buf);
    return ERR_COMPILE_OK;
}
