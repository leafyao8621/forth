#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>

#include <http_server/errcode.h>
#include <http_server/server.h>
#include <containers/eq.h>
#include <containers/hash.h>
#include <fshp/fshp.h>

HTTPServer server;

int index_get(HTTPRequest *request, HTTPResponse *response) {
    int ret =
        HTTPResponse_initialize(
            response,
            HTTP_RESPONSE_200,
            BODY_TYPE_TEXT
        );
    if (ret) {
        return 1;
    }
    size_t memory = 65536;
    size_t lookup = 9500;
    size_t literal = 2000;
    size_t ext = 2000;
    size_t mod = 2000;
    size_t mod_so = 2000;
    size_t interpreted = 2000;
    size_t data_stack = 2000;
    size_t float_stack = 2000;
    size_t return_stack = 2000;
    size_t compiled = 2000;
    ForthVM vm;
    ForthParser parser;
    ForthVMStatus ret_vm = VM_STATUS_OK;
    ForthParserStatus ret_parser = PARSER_STATUS_OK;
    ret_parser = parser_initialize(&parser);
    String buf;
    ret = DArrayChar_initialize(&buf, 1000);
    if (ret) {
        fprintf(stderr, "%s\n", "Out of memory");
        return 1;
    }
    bool success = false;
    FILE *fin = 0;
    ret_vm =
        vm_initialize(
            &vm,
            memory,
            lookup,
            literal,
            ext,
            mod,
            mod_so,
            interpreted,
            data_stack,
            float_stack,
            return_stack,
            compiled
        );
    vm_reset(&vm);
    char template[] = "/tmp/fshps_XXXXXXXX";
    int ofd = mkstemp(template);
    if (ofd == -1) {
        DArrayChar_finalize(&buf);
        vm_finalize(&vm);
        parser_finalize(&parser);
        return 1;
    }
    int stdout_backup = dup(STDOUT_FILENO);
    if (stdout_backup == -1) {
        close(ofd);
        DArrayChar_finalize(&buf);
        vm_finalize(&vm);
        parser_finalize(&parser);
        return 1;
    }
    if (dup2(ofd, STDOUT_FILENO) == -1) {
        close(ofd);
        DArrayChar_finalize(&buf);
        vm_finalize(&vm);
        parser_finalize(&parser);
        return 1;
    }
    FSHPStatus status =
        fshp_process_file("script/index.fshp", &vm, &parser, &buf);
    fflush(stdout);
    if (dup2(stdout_backup, STDOUT_FILENO) == -1) {
        close(ofd);
        DArrayChar_finalize(&buf);
        vm_finalize(&vm);
        parser_finalize(&parser);
        return 1;
    }
    if (status) {
        ret = DArrayChar_push_back_batch(&response->body.text, "Invalid!", 9);
        close(ofd);
        DArrayChar_finalize(&buf);
        vm_finalize(&vm);
        parser_finalize(&parser);
        if (ret) {
            return 1;
        }
        return 0;
    }
    char buf_in[1000];
    ssize_t sz = 0;
    lseek(ofd, 0, SEEK_SET);
    for (; sz = read(ofd, buf_in, 1000);) {
        ret = DArrayChar_push_back_batch(&response->body.text, buf_in, sz);
        if (ret) {
            DArrayChar_finalize(&buf);
            vm_finalize(&vm);
            parser_finalize(&parser);
            close(ofd);
            return 1;
        }
    }
    close(ofd);
    char chr = 0;
    ret = DArrayChar_push_back(&response->body.text, &chr);
    if (ret) {
        DArrayChar_finalize(&buf);
        vm_finalize(&vm);
        parser_finalize(&parser);
        return 1;
    }
    DArrayChar_finalize(&buf);
    vm_finalize(&vm);
    parser_finalize(&parser);
    return 0;
}

void sig_handler(int sig) {
    printf("Received signal %d\n", sig);
    int ret = HTTPServer_finalize(&server);
    printf("retcode: %d\nmsg: %s\n", ret, http_server_errcode_lookup[ret]);
    exit(0);
}

void serve(int port) {
    int ret = HTTPServer_initialize(&server, 8000);
    printf("retcode: %d\nmsg: %s\n", ret, http_server_errcode_lookup[ret]);
    ret =
        HTTPServer_set_route(
            &server,
            "/ ",
            HTTP_METHOD_GET,
            index_get
        );
    printf("retcode: %d\nmsg: %s\n", ret, http_server_errcode_lookup[ret]);
    signal(SIGABRT, sig_handler);
    signal(SIGFPE, sig_handler);
    signal(SIGILL, sig_handler);
    signal(SIGINT, sig_handler);
    signal(SIGSEGV, sig_handler);
    signal(SIGTERM, sig_handler);
    HTTPServer_start(&server);
}
