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
    String path_buf;
    if (DArrayChar_initialize(&path_buf, 1000)) {
        parser->status = PARSER_STATUS_END;
        DArrayChar_finalize(&path);
        return PARSER_STATUS_OUT_OF_MEMORY;
    }
    if (DArrayChar_push_back_batch(&path_buf, path.data, path.size)) {
        parser->status = PARSER_STATUS_END;
        DArrayChar_finalize(&path);
        DArrayChar_finalize(&path_buf);
        return PARSER_STATUS_OUT_OF_MEMORY;
    }
    char *moddef_raw = "moddef";
    if (
        DArrayChar_push_back_batch(
            &path_buf, moddef_raw, strlen(moddef_raw) + 1)) {
        parser->status = PARSER_STATUS_END;
        DArrayChar_finalize(&path);
        DArrayChar_finalize(&path_buf);
        return PARSER_STATUS_OUT_OF_MEMORY;
    }
    FILE *fin = fopen(path_buf.data, "r");
    if (!fin) {
        parser->status = PARSER_STATUS_END;
        DArrayChar_finalize(&path);
        DArrayChar_finalize(&path_buf);
        return PARSER_STATUS_MOD_LOAD;
    }
    uint8_t *buf_name = malloc(1000);
    if (!buf_name) {
        parser->status = PARSER_STATUS_END;
        DArrayChar_finalize(&path);
        DArrayChar_finalize(&path_buf);
        fclose(fin);
        return PARSER_STATUS_OUT_OF_MEMORY;
    }
    size_t buf_name_size = 1000;
    uint8_t *buf_func = malloc(1000);
    if (!buf_func) {
        parser->status = PARSER_STATUS_END;
        DArrayChar_finalize(&path);
        DArrayChar_finalize(&path_buf);
        fclose(fin);
        return PARSER_STATUS_OUT_OF_MEMORY;
    }
    size_t buf_func_size = 1000;
    for (; !feof(fin);) {
        uint8_t mode = 0;
        size_t read = fread(&mode, sizeof(uint8_t), 1, fin);
        if (feof(fin) || read != sizeof(uint8_t)) {
            break;
        }
        if (vm->lookup_cur == vm->lookup_end) {
            parser->status = PARSER_STATUS_END;
            DArrayChar_finalize(&path);
            DArrayChar_finalize(&path_buf);
            fclose(fin);
            free(buf_name);
            free(buf_func);
            return PARSER_STATUS_LOOKUP_OVERFLOW;
        }
        *(vm->lookup_cur++) = mode;
        size_t sz_name = 0;
        read = fread(&sz_name, sizeof(size_t), 1, fin);
        if (feof(fin) || read != sizeof(size_t)) {
            break;
        }
        if (sz_name > buf_name_size) {
            buf_name_size = sz_name;
            buf_name = realloc(buf_name, buf_name_size);
            if (!buf_name) {
                parser->status = PARSER_STATUS_END;
                DArrayChar_finalize(&path);
                DArrayChar_finalize(&path_buf);
                fclose(fin);
                free(buf_func);
                return PARSER_STATUS_OUT_OF_MEMORY;
            }
        }
        read = fread(buf_name, 1, sz_name, fin);
        if (feof(fin) || read != sz_name) {
            break;
        }
        size_t sz = 0;
        read = fread(&sz, sizeof(size_t), 1, fin);
        if (feof(fin) || read != sizeof(uint8_t)) {
            break;
        }
        if (sz > buf_func_size) {
            buf_func_size = sz;
            buf_func = realloc(buf_func, buf_func_size);
            if (!buf_name) {
                parser->status = PARSER_STATUS_END;
                DArrayChar_finalize(&path);
                DArrayChar_finalize(&path_buf);
                fclose(fin);
                free(buf_name);
                return PARSER_STATUS_OUT_OF_MEMORY;
            }
        }
        read = fread(buf_func, 1, sz, fin);
        if (feof(fin) || read != sz) {
            break;
        }
        if (mode & VM_LOOKUP_META_CALLEXT) {
            if (vm->lookup_cur + sizeof(uintptr_t) > vm->lookup_end) {
                parser->status = PARSER_STATUS_END;
                DArrayChar_finalize(&path);
                DArrayChar_finalize(&path_buf);
                fclose(fin);
                free(buf_name);
                free(buf_func);
                return PARSER_STATUS_OUT_OF_MEMORY;
            }
            *(void**)vm->lookup_cur =
                dlsym(*(void**)vm->mod_cur, (const char*)buf_func);
            if (!vm->lookup_cur) {
                parser->status = PARSER_STATUS_END;
                DArrayChar_finalize(&path);
                DArrayChar_finalize(&path_buf);
                fclose(fin);
                free(buf_name);
                free(buf_func);
                return PARSER_STATUS_MOD_LOAD;
            }
            vm->lookup_cur += sizeof(uintptr_t);
            if (mode & VM_LOOKUP_META_PARSEEXT) {
                sz = 0;
                read = fread(&sz, sizeof(size_t), 1, fin);
                if (feof(fin) || read != sizeof(size_t)) {
                    break;
                }
                if (sz > buf_func_size) {
                    buf_func_size = sz;
                    buf_func = realloc(buf_func, buf_func_size);
                    if (!buf_name) {
                        parser->status = PARSER_STATUS_END;
                        DArrayChar_finalize(&path);
                        DArrayChar_finalize(&path_buf);
                        fclose(fin);
                        free(buf_name);
                        return PARSER_STATUS_OUT_OF_MEMORY;
                    }
                }
                read = fread(buf_func, 1, sz, fin);
                if (feof(fin) || read != sizeof(sz)) {
                    break;
                }
                if (vm->lookup_cur + sizeof(uintptr_t) > vm->lookup_end) {
                    parser->status = PARSER_STATUS_END;
                    DArrayChar_finalize(&path);
                    DArrayChar_finalize(&path_buf);
                    fclose(fin);
                    free(buf_name);
                    free(buf_func);
                    return PARSER_STATUS_OUT_OF_MEMORY;
                }
                *(void**)vm->lookup_cur =
                    dlsym(*(void**)vm->mod_cur, (const char*)buf_func);
                if (!vm->lookup_cur) {
                    parser->status = PARSER_STATUS_END;
                    DArrayChar_finalize(&path);
                    DArrayChar_finalize(&path_buf);
                    fclose(fin);
                    free(buf_name);
                    free(buf_func);
                    return PARSER_STATUS_MOD_LOAD;
                }
                vm->lookup_cur += sizeof(uintptr_t);
            }
        } else {
            if (vm->lookup_cur + sizeof(uintptr_t) > vm->lookup_end) {
                parser->status = PARSER_STATUS_END;
                DArrayChar_finalize(&path);
                DArrayChar_finalize(&path_buf);
                fclose(fin);
                free(buf_name);
                free(buf_func);
                return PARSER_STATUS_OUT_OF_MEMORY;
            }
            vm->lookup_cur += sizeof(uintptr_t);
            if (vm->lookup_cur + sizeof(uintptr_t) > vm->lookup_end) {
                parser->status = PARSER_STATUS_END;
                DArrayChar_finalize(&path);
                DArrayChar_finalize(&path_buf);
                fclose(fin);
                free(buf_name);
                free(buf_func);
                return PARSER_STATUS_OUT_OF_MEMORY;
            }
            *(void**)vm->lookup_cur =
                dlsym(*(void**)vm->mod_cur, (const char*)buf_func);
            if (!vm->lookup_cur) {
                parser->status = PARSER_STATUS_END;
                DArrayChar_finalize(&path);
                DArrayChar_finalize(&path_buf);
                fclose(fin);
                free(buf_name);
                free(buf_func);
                return PARSER_STATUS_MOD_LOAD;
            }
            vm->lookup_cur += sizeof(uintptr_t);
        }
        if (vm->lookup_cur + sz_name > vm->lookup_end) {
            parser->status = PARSER_STATUS_END;
            DArrayChar_finalize(&path);
            DArrayChar_finalize(&path_buf);
            fclose(fin);
            free(buf_name);
            free(buf_func);
            return PARSER_STATUS_OUT_OF_MEMORY;
        }
        memcpy(vm->lookup_cur, buf_name, sz_name);
        vm->lookup_cur += sz_name;
    }
    vm->mod_cur += sizeof(uintptr_t);
    DArrayChar_finalize(&path);
    DArrayChar_finalize(&path_buf);
    free(buf_name);
    free(buf_func);
    fclose(fin);
    return PARSER_STATUS_OK;
}
