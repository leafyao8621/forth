#include <stdio.h>

#include <containers/dstring.h>

#include "compile.h"

const char *get_1 =
    "#include <fshps/fshps.h>\n\n"
    "int ";

const char *get_2 =
    "_get(HTTPRequest *request, HTTPResponse *response) {\n"
    "    if (!request) {\n"
    "        return 1;\n"
    "    }\n"
    "    int ret =\n"
    "        HTTPResponse_initialize(\n"
    "            response,\n"
    "            HTTP_RESPONSE_200,\n"
    "            BODY_TYPE_TEXT\n"
    "        );\n"
    "    if (ret) {\n"
    "        return 1;\n"
    "    }\n"
    "\n"
    "    ForthVM vm;\n"
    "    ForthParser parser;\n"
    "    ForthVMStatus ret_vm = VM_STATUS_OK;\n"
    "    ForthParserStatus ret_parser = PARSER_STATUS_OK;\n"
    "    ret_parser = parser_initialize(&parser);\n"
    "    if (ret_parser) {\n"
    "        fputs(\"Parser Init\\n\", stderr);\n"
    "    }\n"
    "    String buf;\n"
    "    ret = DArrayChar_initialize(&buf, 1000);\n"
    "    if (ret) {\n"
    "        fprintf(stderr, \"%s\\n\", \"Out of memory\");\n"
    "        return 1;\n"
    "    }\n"
    "    ret_vm =\n"
    "        vm_initialize(\n"
    "            &vm,\n"
    "            memory,\n"
    "            lookup,\n"
    "            literal,\n"
    "            ext,\n"
    "            mod,\n"
    "            mod_so,\n"
    "            interpreted,\n"
    "            data_stack,\n"
    "            float_stack,\n"
    "            return_stack,\n"
    "            compiled\n"
    "        );\n"
    "    if (ret_vm) {\n"
    "        fputs(\"VM Init\\n\", stderr);\n"
    "    }\n"
    "    vm_reset(&vm);\n"
    "    char template[] = \"/tmp/fshps_XXXXXXXX\";\n"
    "    int ofd = mkstemp(template);\n"
    "    if (ofd == -1) {\n"
    "        DArrayChar_finalize(&buf);\n"
    "        vm_finalize(&vm);\n"
    "        parser_finalize(&parser);\n"
    "        return 1;\n"
    "    }\n"
    "    int stdout_backup = dup(STDOUT_FILENO);\n"
    "    if (stdout_backup == -1) {\n"
    "        close(ofd);\n"
    "        DArrayChar_finalize(&buf);\n"
    "        vm_finalize(&vm);\n"
    "        parser_finalize(&parser);\n"
    "        return 1;\n"
    "    }\n"
    "    if (dup2(ofd, STDOUT_FILENO) == -1) {\n"
    "        close(ofd);\n"
    "        DArrayChar_finalize(&buf);\n"
    "        vm_finalize(&vm);\n"
    "        parser_finalize(&parser);\n"
    "        return 1;\n"
    "    }\n"
    "    FSHPStatus status =\n"
    "        fshp_process_file(\"";

const char *get_3 =
    "\", &vm, &parser, &buf);\n"
    "    fflush(stdout);\n"
    "    if (dup2(stdout_backup, STDOUT_FILENO) == -1) {\n"
    "        close(ofd);\n"
    "        DArrayChar_finalize(&buf);\n"
    "        vm_finalize(&vm);\n"
    "        parser_finalize(&parser);\n"
    "        return 1;\n"
    "    }\n"
    "    if (status) {\n"
    "        printf(\"FSHP Status: %d %s\\n\", status, fshp_status_lookup[status]);\n"
    "        ret = DArrayChar_push_back_batch(&response->body.text, \"Invalid!\", 9);\n"
    "        close(ofd);\n"
    "        DArrayChar_finalize(&buf);\n"
    "        vm_finalize(&vm);\n"
    "        parser_finalize(&parser);\n"
    "        if (ret) {\n"
    "            return 1;\n"
    "        }\n"
    "        return 0;\n"
    "    }\n"
    "    char buf_in[1000];\n"
    "    ssize_t sz = 0;\n"
    "    lseek(ofd, 0, SEEK_SET);\n"
    "    for (; (sz = read(ofd, buf_in, 1000));) {\n"
    "        ret = DArrayChar_push_back_batch(&response->body.text, buf_in, sz);\n"
    "        if (ret) {\n"
    "            DArrayChar_finalize(&buf);\n"
    "            vm_finalize(&vm);\n"
    "            parser_finalize(&parser);\n"
    "            close(ofd);\n"
    "            return 1;\n"
    "        }\n"
    "    }\n"
    "    close(ofd);\n"
    "    char chr = 0;\n"
    "    ret = DArrayChar_push_back(&response->body.text, &chr);\n"
    "    if (ret) {\n"
    "        DArrayChar_finalize(&buf);\n"
    "        vm_finalize(&vm);\n"
    "        parser_finalize(&parser);\n"
    "        return 1;\n"
    "    }\n"
    "    DArrayChar_finalize(&buf);\n"
    "    vm_finalize(&vm);\n"
    "    parser_finalize(&parser);\n"
    "    return 0;\n"
    "}\n";

ErrCompile fshps_compile(void) {
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
    String header_buf;
    if (DArrayChar_initialize(&header_buf, 80)) {
        fclose(fin);
        fclose(fout);
        DArrayChar_finalize(&buf);
        return ERR_COMPILE_OUT_OF_MEMORY;
    }
    String fn;
    if (DArrayChar_initialize(&fn, 80)) {
        fclose(fin);
        fclose(fout);
        DArrayChar_finalize(&buf);
        DArrayChar_finalize(&header_buf);
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
                DArrayChar_finalize(&header_buf);
                DArrayChar_finalize(&fn);
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
            DArrayChar_finalize(&header_buf);
            DArrayChar_finalize(&fn);
            return ERR_COMPILE_OUT_OF_MEMORY;
        }
        if (is_header) {
            if (!strcmp(buf.data, "KEY")) {
                header = HEADER_KEY;
                fwrite(&header, 1, 1, fout);
            } else if (!strcmp(buf.data, "URL")) {
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
                DArrayChar_finalize(&header_buf);
                DArrayChar_finalize(&fn);
                return ERR_COMPILE_INVALID_HEADER;
            }
        } else {
            switch (header) {
            case HEADER_KEY:
                fwrite(&buf.size, sizeof(size_t), 1, fout);
                fwrite(buf.data, buf.size, 1, fout);
                DArrayChar_clear(&header_buf);
                if (
                    DArrayChar_push_back_batch(
                        &header_buf, buf.data, buf.size)) {
                    fclose(fin);
                    fclose(fout);
                    DArrayChar_finalize(&buf);
                    DArrayChar_finalize(&header_buf);
                    DArrayChar_finalize(&fn);
                    return ERR_COMPILE_OUT_OF_MEMORY;
                }
                break;
            case HEADER_URL:
                if (buf.size >= 2 && buf.data[buf.size - 2] != ' ') {
                    buf.data[buf.size - 1] = ' ';
                    if (DArrayChar_push_back(&buf, &zero)) {
                        fclose(fin);
                        fclose(fout);
                        DArrayChar_finalize(&buf);
                        DArrayChar_finalize(&header_buf);
                        DArrayChar_finalize(&fn);
                        return ERR_COMPILE_OUT_OF_MEMORY;
                    }
                }
                fwrite(&buf.size, sizeof(size_t), 1, fout);
                fwrite(buf.data, buf.size, 1, fout);
                break;
            case HEADER_GET:
                DArrayChar_clear(&fn);
                if (
                    DArrayChar_push_back_batch(
                        &fn, "src/", 4)) {
                    fclose(fin);
                    fclose(fout);
                    DArrayChar_finalize(&buf);
                    DArrayChar_finalize(&header_buf);
                    DArrayChar_finalize(&fn);
                    return ERR_COMPILE_OUT_OF_MEMORY;
                }
                if (
                    DArrayChar_push_back_batch(
                        &fn, header_buf.data, header_buf.size - 1)) {
                    fclose(fin);
                    fclose(fout);
                    DArrayChar_finalize(&buf);
                    DArrayChar_finalize(&header_buf);
                    DArrayChar_finalize(&fn);
                    return ERR_COMPILE_OUT_OF_MEMORY;
                }
                if (
                    DArrayChar_push_back_batch(
                        &fn, "_get.c", 7)) {
                    fclose(fin);
                    fclose(fout);
                    DArrayChar_finalize(&buf);
                    DArrayChar_finalize(&header_buf);
                    DArrayChar_finalize(&fn);
                    return ERR_COMPILE_OUT_OF_MEMORY;
                }
                FILE *fout_temp = fopen(fn.data, "w");
                if (!fout_temp) {
                    fclose(fin);
                    fclose(fout);
                    DArrayChar_finalize(&buf);
                    DArrayChar_finalize(&header_buf);
                    DArrayChar_finalize(&fn);
                    return ERR_COMPILE_OPEN;
                }
                fputs(get_1, fout_temp);
                fputs(header_buf.data, fout_temp);
                fputs(get_2, fout_temp);
                fputs(buf.data, fout_temp);
                fputs(get_3, fout_temp);
                fclose(fout_temp);
                break;
            }
        }
    }
    fclose(fin);
    fclose(fout);
    DArrayChar_finalize(&buf);
    DArrayChar_finalize(&header_buf);
    DArrayChar_finalize(&fn);
    return ERR_COMPILE_OK;
}
