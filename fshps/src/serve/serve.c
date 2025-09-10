#include <stdio.h>
#include <signal.h>

#include <fshps/fshps.h>

HTTPServer server;

size_t memory;
size_t lookup;
size_t literal;
size_t ext;
size_t mod;
size_t mod_so;
size_t interpreted;
size_t data_stack;
size_t float_stack;
size_t return_stack;
size_t compiled;

int index_get(HTTPRequest *request, HTTPResponse *response) {
    if (!request) {
        return 1;
    }
    int ret =
        HTTPResponse_initialize(
            response,
            HTTP_RESPONSE_200,
            BODY_TYPE_TEXT
        );
    if (ret) {
        return 1;
    }

    ForthVM vm;
    ForthParser parser;
    ForthVMStatus ret_vm = VM_STATUS_OK;
    ForthParserStatus ret_parser = PARSER_STATUS_OK;
    ret_parser = parser_initialize(&parser);
    if (ret_parser) {
        fputs("Parser Init\n", stderr);
    }
    String buf;
    ret = DArrayChar_initialize(&buf, 1000);
    if (ret) {
        fprintf(stderr, "%s\n", "Out of memory");
        return 1;
    }
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
    if (ret_vm) {
        fputs("VM Init\n", stderr);
    }
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
    for (; (sz = read(ofd, buf_in, 1000));) {
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

void serve(
    int port,
    size_t memory_in,
    size_t lookup_in,
    size_t literal_in,
    size_t ext_in,
    size_t mod_in,
    size_t mod_so_in,
    size_t interpreted_in,
    size_t data_stack_in,
    size_t float_stack_in,
    size_t return_stack_in,
    size_t compiled_in) {
    memory = memory_in;
    lookup = lookup_in;
    literal = literal_in;
    ext = ext_in;
    mod = mod_in;
    mod_so = mod_so_in;
    interpreted = interpreted_in;
    data_stack = data_stack_in;
    float_stack = float_stack_in;
    return_stack = return_stack_in;
    compiled = compiled_in;

    int ret = HTTPServer_initialize(&server, port);
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
