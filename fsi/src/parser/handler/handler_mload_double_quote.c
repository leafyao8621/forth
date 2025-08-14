#include <dlfcn.h>

#include <fsi/util/status.h>
#include <fsi/util/parser_util.h>
#include <fsi/parser/parser.h>

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
        if (feof(fin) || read != 1) {
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
        if (feof(fin) || read != 1) {
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
        if (feof(fin) || read != 1) {
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
                if (feof(fin) || read != 1) {
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
    fclose(fin);
    free(buf_func);

    DArrayChar_clear(&path_buf);
    if (DArrayChar_push_back_batch(&path_buf, path.data, path.size)) {
        parser->status = PARSER_STATUS_END;
        DArrayChar_finalize(&path);
        DArrayChar_finalize(&path_buf);
        free(buf_name);
        return PARSER_STATUS_OUT_OF_MEMORY;
    }
    char *load_order_raw = "load_order";
    if (
        DArrayChar_push_back_batch(
            &path_buf, load_order_raw, strlen(load_order_raw) + 1)) {
        parser->status = PARSER_STATUS_END;
        DArrayChar_finalize(&path);
        DArrayChar_finalize(&path_buf);
        free(buf_name);
        return PARSER_STATUS_OUT_OF_MEMORY;
    }
    String path_buf_script;
    if (DArrayChar_initialize(&path_buf_script, 1000)) {
        parser->status = PARSER_STATUS_END;
        DArrayChar_finalize(&path);
        DArrayChar_finalize(&path_buf);
        free(buf_name);
        return PARSER_STATUS_OUT_OF_MEMORY;
    }
    fin = fopen(path_buf.data, "r");
    if (!fin) {
        parser->status = PARSER_STATUS_END;
        DArrayChar_finalize(&path);
        DArrayChar_finalize(&path_buf);
        DArrayChar_finalize(&path_buf_script);
        free(buf_name);
        return PARSER_STATUS_MOD_LOAD;
    }
    String script_buf;
    if (DArrayChar_initialize(&script_buf, 1000)) {
        parser->status = PARSER_STATUS_END;
        DArrayChar_finalize(&path);
        DArrayChar_finalize(&path_buf);
        DArrayChar_finalize(&path_buf_script);
        free(buf_name);
        return PARSER_STATUS_OUT_OF_MEMORY;
    }
    ForthParser parser_script;
    ForthParserStatus ret_parser = PARSER_STATUS_OK;
    ret_parser = parser_initialize(&parser_script);
    if (ret_parser) {
        parser->status = PARSER_STATUS_END;
        DArrayChar_finalize(&path);
        DArrayChar_finalize(&path_buf);
        DArrayChar_finalize(&path_buf_script);
        free(buf_name);
        return PARSER_STATUS_OUT_OF_MEMORY;
    }
    ForthVMStatus ret_vm = vm_run(vm, false);
    if (ret_vm) {
        fprintf(
            stderr,
            "Error running\n%s\n",
            vm_status_lookup[ret_vm]
        );
        DArrayChar_finalize(&path);
        DArrayChar_finalize(&path_buf);
        DArrayChar_finalize(&path_buf_script);
        fclose(fin);
        free(buf_name);
        DArrayChar_finalize(&script_buf);
        parser_finalize(&parser_script);
        return 1;
    }
    for (; !feof(fin);) {
        size_t sz_name = 0;
        size_t read = fread(&sz_name, sizeof(size_t), 1, fin);
        if (feof(fin) || read != 1) {
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
                free(buf_name);
                parser_finalize(&parser_script);
                return PARSER_STATUS_OUT_OF_MEMORY;
            }
        }
        read = fread(buf_name, 1, sz_name, fin);
        if (feof(fin) || read != sz_name) {
            break;
        }
        DArrayChar_clear(&path_buf_script);
        if (DArrayChar_push_back_batch(
            &path_buf_script, path.data, path.size)) {
            parser->status = PARSER_STATUS_END;
            DArrayChar_finalize(&path);
            DArrayChar_finalize(&path_buf);
            fclose(fin);
            free(buf_name);
            parser_finalize(&parser_script);
            return PARSER_STATUS_OUT_OF_MEMORY;
        }
        char *script_base = "script/";
        if (
            DArrayChar_push_back_batch(
                &path_buf_script, script_base, strlen(script_base))) {
            parser->status = PARSER_STATUS_END;
            DArrayChar_finalize(&path);
            DArrayChar_finalize(&path_buf);
            fclose(fin);
            free(buf_name);
            parser_finalize(&parser_script);
            return PARSER_STATUS_OUT_OF_MEMORY;
        }
        if (
            DArrayChar_push_back_batch(
                &path_buf_script, (char*)buf_name, buf_name_size)) {
            parser->status = PARSER_STATUS_END;
            DArrayChar_finalize(&path);
            DArrayChar_finalize(&path_buf);
            fclose(fin);
            free(buf_name);
            parser_finalize(&parser_script);
            return PARSER_STATUS_OUT_OF_MEMORY;
        }
        vm_reset(vm);
        FILE *fin_script = fopen(path_buf_script.data, "r");
        if (!fin_script) {
            fprintf(stderr, "Cannot open %s\n", path_buf_script.data);
            DArrayChar_finalize(&path);
            DArrayChar_finalize(&path_buf);
            fclose(fin);
            free(buf_name);
            DArrayChar_finalize(&script_buf);
            parser_finalize(&parser_script);
            return PARSER_STATUS_MOD_LOAD;
        }
        int success = read_file(fin_script, &script_buf);
        fclose(fin_script);
        if (!success) {
            fprintf(stderr, "%s\n", "Out of memory");
            DArrayChar_finalize(&path);
            DArrayChar_finalize(&path_buf);
            DArrayChar_finalize(&path_buf_script);
            fclose(fin);
            free(buf_name);
            DArrayChar_finalize(&script_buf);
            parser_finalize(&parser_script);
            return 1;
        }
        ForthParserStatus ret =
            parser_parse(vm, &parser_script, false, false, script_buf.data);
        if (ret) {
            fprintf(
                stderr,
                "Error parsing\n%s\n",
                parser_status_lookup[ret]
            );
            DArrayChar_finalize(&path);
            DArrayChar_finalize(&path_buf);
            DArrayChar_finalize(&path_buf_script);
            fclose(fin);
            free(buf_name);
            DArrayChar_finalize(&script_buf);
            parser_finalize(&parser_script);
            return 1;
        }
        ForthVMStatus ret_vm = vm_run(vm, false);
        if (ret_vm) {
            fprintf(
                stderr,
                "Error running\n%s\n",
                vm_status_lookup[ret_vm]
            );
            DArrayChar_finalize(&path);
            DArrayChar_finalize(&path_buf);
            DArrayChar_finalize(&path_buf_script);
            fclose(fin);
            free(buf_name);
            DArrayChar_finalize(&script_buf);
            parser_finalize(&parser_script);
            return 1;
        }
    }

    fclose(fin);
    DArrayChar_finalize(&path);
    DArrayChar_finalize(&path_buf);
    DArrayChar_finalize(&path_buf_script);
    DArrayChar_finalize(&script_buf);
    free(buf_name);
    parser_finalize(&parser_script);
    vm_reset(vm);
    return PARSER_STATUS_OK;
}
