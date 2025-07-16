#include <dlfcn.h>

#include <fsi/util/status.h>

#include "handler.h"

int parser_handler_mload_double_quote(
    ForthParser *parser, char **iter, ForthVM *vm) {
    if (!vm) {
        return PARSER_STATUS_OK;
    }
    if (parser->state & PARSER_STATE_COMPILE) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_NOT_IN_INTERPRETATION_MODE;
    }
    DArrayChar_clear(&parser->buf);
    for (++(*iter); **iter && **iter != '"'; ++(*iter)) {
        if (!**iter) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_INCOMPLETE_STRING;
        }
        if (DArrayChar_push_back(&parser->buf, *iter)) {
            parser->status = PARSER_STATUS_END;
            return PARSER_STATUS_MODULE_NAME_OVERFLOW;
        }
    }
    char chr = 0;
    if (DArrayChar_push_back(&parser->buf, &chr)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_MODULE_NAME_OVERFLOW;
    }
    ++(*iter);
    String path;
    if (DArrayChar_initialize(&path, 1000)) {
        parser->status = PARSER_STATUS_END;
        return PARSER_STATUS_OUT_OF_MEMORY;
    }
    char *home = getenv("HOME");
    if (DArrayChar_push_back_batch(&path, home, strlen(home))) {
        parser->status = PARSER_STATUS_END;
        DArrayChar_finalize(&path);
        return PARSER_STATUS_OUT_OF_MEMORY;
    }
    char *fsi_base = "/.local/lib/fsi/";
    if (DArrayChar_push_back_batch(&path, fsi_base, strlen(fsi_base))) {
        parser->status = PARSER_STATUS_END;
        DArrayChar_finalize(&path);
        return PARSER_STATUS_OUT_OF_MEMORY;
    }
    if (
        DArrayChar_push_back_batch(
            &path, parser->buf.data, parser->buf.size - 1)) {
        parser->status = PARSER_STATUS_END;
        DArrayChar_finalize(&path);
        return PARSER_STATUS_OUT_OF_MEMORY;
    }
    chr = '/';
    if (DArrayChar_push_back(&path, &chr)) {
        parser->status = PARSER_STATUS_END;
        DArrayChar_finalize(&path);
        return PARSER_STATUS_OUT_OF_MEMORY;
    }
    char *mod_so = (char*)vm->mod_so_cur;
    if (vm->mod_so_cur + path.size > vm->mod_so_end) {
        parser->status = PARSER_STATUS_END;
        DArrayChar_finalize(&path);
        return PARSER_STATUS_OUT_OF_MEMORY;
    }
    memcpy(vm->mod_so_cur, path.data, path.size);
    vm->mod_so_cur += path.size;
    char *so_raw = "lib/mod.so";
    if (vm->mod_so_cur + strlen(so_raw) + 1 > vm->mod_so_end) {
        parser->status = PARSER_STATUS_END;
        DArrayChar_finalize(&path);
        return PARSER_STATUS_OUT_OF_MEMORY;
    }
    strcpy((char*)vm->mod_so_cur, so_raw);
    vm->mod_so_cur += strlen(so_raw) + 1;
    if (vm->mod_cur + sizeof(uintptr_t) > vm->mod_end) {
        parser->status = PARSER_STATUS_END;
        DArrayChar_finalize(&path);
        return PARSER_STATUS_MOD_OVERFLOW;
    }
    *(void**)vm->mod_cur = dlopen(mod_so, RTLD_LAZY);
    if (!*(void**)vm->mod_cur) {
        parser->status = PARSER_STATUS_END;
        DArrayChar_finalize(&path);
        return PARSER_STATUS_MOD_LOAD;
    }
    vm->mod_cur += sizeof(uintptr_t);
    DArrayChar_finalize(&path);
    return PARSER_STATUS_OK;
}
