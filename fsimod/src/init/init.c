#include <string.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "init.h"

const char *hello_parse =
    "#include <fsi/vm/vm.h>\n"
    "#include <fsi/parser/parser.h>\n\n"
    "int parser_handler_hello(ForthParser *parser, ForthVM *vm) {\n"
    "    if (!parser || !vm) {\n"
    "        return PARSER_STATUS_OK;\n"
    "    }\n"
    "    puts(\"Parsing hello\");\n"
    "    return PARSER_STATUS_OK;\n"
    "}\n";

const char *hello_run =
    "#include <fsi/vm/vm.h>\n"
    "#include <fsi/parser/parser.h>\n\n"
    "int vm_ext_hello(ForthVM *vm) {\n"
    "    if (!vm) {\n"
    "        return PARSER_STATUS_OK;\n"
    "    }\n"
    "    puts(\"Running hello\");\n"
    "    return PARSER_STATUS_OK;\n"
    "}\n";

const char *hello1 =
    ": init .\"Hello 1 Loaded\" cr ;\n"
    "init\n"
    ": hello.1 .\" Hello 1\" cr ;\n";
const char *hello2 =
    ": init .\"Hello 2 Loaded\" cr ;\n"
    "init\n"
    ": hello.2 .\" Hello 2\" cr ;\n";

const char *gitignore =
    "*.o\n"
    "*.so\n"
    "moddef\n"
    ".vscode\n";

const char *load_order =
    "hello_2.fs\n"
    "hello_1.fs\n";

const char *moddef =
    "pr\n"
    "hello\n"
    "parser_handler_hello\n"
    "vm_ext_hello\n";

ErrInit init(void) {
    if (mkdir("src", 0755)) {
        return ERR_INIT_MKDIR;
    }
    int fd = creat("src/hello_parse.c", 0755);
    ssize_t sz = write(fd, hello_parse, strlen(hello_parse));
    if (sz == -1) {
        close(fd);
        return ERR_INIT_WRITE;
    }
    close(fd);
    fd = creat("src/hello_run.c", 0755);
    sz = write(fd, hello_run, strlen(hello_run));
    if (sz == -1) {
        close(fd);
        return ERR_INIT_WRITE;
    }
    close(fd);
    if (mkdir("lib", 0755)) {
        return ERR_INIT_MKDIR;
    }
    fd = creat("lib/.gitkeep", 0755);
    if (!fd) {
        return ERR_INIT_CREATE;
    }
    close(fd);
    if (mkdir("script", 0755)) {
        return ERR_INIT_MKDIR;
    }
    fd = creat("script/.gitkeep", 0755);
    if (!fd) {
        return ERR_INIT_CREATE;
    }
    close(fd);
    fd = creat("script/hello_1.fs", 0755);
    sz = write(fd, hello1, strlen(hello1));
    if (sz == -1) {
        close(fd);
        return ERR_INIT_WRITE;
    }
    close(fd);
    fd = creat("script/hello_2.fs", 0755);
    sz = write(fd, hello2, strlen(hello2));
    if (sz == -1) {
        close(fd);
        return ERR_INIT_WRITE;
    }
    close(fd);
    fd = creat(".gitignore", 0755);
    sz = write(fd, gitignore, strlen(gitignore));
    if (sz == -1) {
        close(fd);
        return ERR_INIT_WRITE;
    }
    close(fd);
    fd = creat("load_order.txt", 0755);
    if (!fd) {
        return ERR_INIT_CREATE;
    }
    sz = write(fd, load_order, strlen(load_order));
    if (sz == -1) {
        close(fd);
        return ERR_INIT_WRITE;
    }
    close(fd);
    fd = creat("moddef.txt", 0755);
    if (!fd) {
        return ERR_INIT_CREATE;
    }
    sz = write(fd, moddef, strlen(moddef));
    if (sz == -1) {
        close(fd);
        return ERR_INIT_WRITE;
    }
    close(fd);
    return ERR_INIT_OK;
}
