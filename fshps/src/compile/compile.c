#include <stdio.h>

#include <containers/dstring.h>

#include "compile.h"

ErrCompile compile(void) {
    FILE *fin = fopen("route.txt", "r");
    if (!fin) {
        return ERR_COMPILE_OPEN;
    }
    FILE *fout = fopen("route", "w");
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
    bool is_header = true;
    char header = 0;
    for (; !feof(fin); is_header = !is_header) {
        DArrayChar_clear(&buf);
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
        if (is_header) {
            if (!strcmp(buf.data, "URL")) {
                header = HEADER_URL;
                fwrite(&header, 1, 1, fout);
            } else if (!strcmp(buf.data, "GET")) {
                header = HEADER_GET;
                fwrite(&header, 1, 1, fout);
            } else if (!strcmp(buf.data, "HEAD")) {
                header = HEADER_HEAD;
                fwrite(&header, 1, 1, fout);
            } else if (!strcmp(buf.data, "POST")) {
                header = HEADER_POST;
                fwrite(&header, 1, 1, fout);
            } else if (!strcmp(buf.data, "PUT")) {
                header = HEADER_PUT;
                fwrite(&header, 1, 1, fout);
            } else if (!strcmp(buf.data, "DELETE")) {
                header = HEADER_DELETE;
                fwrite(&header, 1, 1, fout);
            } else if (!strcmp(buf.data, "CONNECT")) {
                header = HEADER_CONNECT;
                fwrite(&header, 1, 1, fout);
            } else if (!strcmp(buf.data, "OPTIONS")) {
                header = HEADER_OPTIONS;
                fwrite(&header, 1, 1, fout);
            } else if (!strcmp(buf.data, "TRACE")) {
                header = HEADER_TRACE;
                fwrite(&header, 1, 1, fout);
            } else if (!strcmp(buf.data, "PATCH")) {
                header = HEADER_PATCH;
                fwrite(&header, 1, 1, fout);
            } else {
                fclose(fin);
                fclose(fout);
                DArrayChar_finalize(&buf);
                return ERR_COMPILE_INVALID_HEADER;
            }
        } else {
            fwrite(&buf.size, sizeof(size_t), 1, fout);
            fwrite(buf.data, buf.size, 1, fout);
        }
    }
    fclose(fin);
    fclose(fout);
    DArrayChar_finalize(&buf);
    return ERR_COMPILE_OK;
}
