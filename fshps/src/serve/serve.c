#include <stdio.h>
#include <signal.h>

#include <dlfcn.h>

#include <fshps/fshps.h>

#include "../compile/compile.h"

static HTTPServer server;
static void *lib = 0;

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

void sig_handler(int sig) {
    printf("Received signal %d\n", sig);
    int ret = HTTPServer_finalize(&server);
    printf("retcode: %d\nmsg: %s\n", ret, http_server_errcode_lookup[ret]);
    if (lib) {
        dlclose(lib);
    }
    exit(0);
}

void fshps_serve(
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
    FILE *fin = fopen("route", "rb");
    if (!fin) {
        puts("Cannot open route");
        HTTPServer_finalize(&server);
        return;
    }
    String key;
    String url;
    String fn;
    if (DArrayChar_initialize(&key, 80)) {
        puts("Out of memory");
        HTTPServer_finalize(&server);
        fclose(fin);
        return;
    }
    if (DArrayChar_initialize(&url, 80)) {
        puts("Out of memory");
        HTTPServer_finalize(&server);
        fclose(fin);
        DArrayChar_finalize(&key);
        return;
    }
    if (DArrayChar_initialize(&fn, 80)) {
        puts("Out of memory");
        HTTPServer_finalize(&server);
        fclose(fin);
        DArrayChar_finalize(&key);
        DArrayChar_finalize(&url);
        return;
    }
    lib = dlopen("./lib/handler.so", RTLD_LAZY);
    if (!lib) {
        printf("Cannot open library: %s\n", dlerror());
        HTTPServer_finalize(&server);
        fclose(fin);
        DArrayChar_finalize(&key);
        DArrayChar_finalize(&url);
        DArrayChar_finalize(&fn);
        return;
    }
    printf("Library loaded at %p\n", lib);
    for (; !feof(fin);) {
        char header = 0;
        ret = fread(&header, 1, 1, fin);
        size_t sz = 0;
        switch (header) {
        case HEADER_KEY:
            ret = fread(&sz, sizeof(size_t), 1, fin);
            DArrayChar_clear(&key);
            if (DArrayChar_expand(&key, sz, false)) {
                puts("Out of memory");
                HTTPServer_finalize(&server);
                fclose(fin);
                DArrayChar_finalize(&key);
                DArrayChar_finalize(&url);
                DArrayChar_finalize(&fn);
                return;
            }
            ret = fread(key.data, 1, sz, fin);
            break;
        case HEADER_URL:
            ret = fread(&sz, sizeof(size_t), 1, fin);
            DArrayChar_clear(&url);
            if (DArrayChar_expand(&url, sz, false)) {
                puts("Out of memory");
                HTTPServer_finalize(&server);
                fclose(fin);
                DArrayChar_finalize(&url);
                DArrayChar_finalize(&url);
                DArrayChar_finalize(&fn);
                return;
            }
            ret = fread(url.data, 1, sz, fin);
            break;
        case HEADER_GET:
            DArrayChar_clear(&fn);
            if (DArrayChar_push_back_batch(&fn, key.data, key.size - 1)) {
                puts("Out of memory");
                HTTPServer_finalize(&server);
                fclose(fin);
                DArrayChar_finalize(&key);
                DArrayChar_finalize(&url);
                DArrayChar_finalize(&fn);
                return;
            }
            if (DArrayChar_push_back_batch(&fn, "_get", 5)) {
                puts("Out of memory");
                HTTPServer_finalize(&server);
                fclose(fin);
                DArrayChar_finalize(&key);
                DArrayChar_finalize(&url);
                DArrayChar_finalize(&fn);
                return;
            }
            HTTPRequestHandler handler;
            *(void**)&handler = dlsym(lib, fn.data);
            printf(
                "Setting route %s to handler %s at %p\n",
                url.data,
                fn.data,
                *(void**)&handler
            );
            ret =
                HTTPServer_set_route(
                    &server,
                    url.data,
                    HTTP_METHOD_GET,
                    handler
                );
            printf(
                "retcode: %d\nmsg: %s\n",
                ret,
                http_server_errcode_lookup[ret]
            );
            if (ret) {
                HTTPServer_finalize(&server);
                fclose(fin);
                DArrayChar_finalize(&key);
                DArrayChar_finalize(&url);
                DArrayChar_finalize(&fn);
                return;
            }
            break;
        }
    }
    fclose(fin);
    DArrayChar_finalize(&key);
    DArrayChar_finalize(&url);
    DArrayChar_finalize(&fn);
    signal(SIGABRT, sig_handler);
    signal(SIGFPE, sig_handler);
    signal(SIGILL, sig_handler);
    signal(SIGINT, sig_handler);
    signal(SIGSEGV, sig_handler);
    signal(SIGTERM, sig_handler);
    HTTPServer_start(&server);
}
