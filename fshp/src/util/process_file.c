#include <fshp/fshp.h>
#include <fsi/util/status.h>

static FSHPStatus run_code(
    FILE *fin, ForthParser *parser, ForthVM *vm, String *buf, size_t idx) {
    bool end_tag = false;
    for (char i = fgetc(fin); !feof(fin); i = fgetc(fin)) {
        int ret = DArrayChar_push_back(buf, &i);
        if (ret) {
            return FSHP_STATUS_OUT_OF_MEMORY;
        }
        if (!memcmp(buf->data + buf->size - 2, "?>", 2)) {
            end_tag = true;
            break;
        }
    }
    if (end_tag) {
        buf->data[buf->size - 2] = 0;
    } else {
        char chr = 0;
        int ret = DArrayChar_push_back(buf, &chr);
        if (ret) {
            return FSHP_STATUS_OUT_OF_MEMORY;
        }
    }
    ForthParserStatus ret_parser =
        parser_parse(vm, parser, false, false, buf->data + idx);
    if (ret_parser) {
        switch (ret_parser) {
        case PARSER_STATUS_END_REPL:
            return FSHP_PARSER_STATUS_END_REPL;
        case PARSER_STATUS_OUT_OF_MEMORY:
            return FSHP_PARSER_STATUS_OUT_OF_MEMORY;
        case PARSER_STATUS_INTERPRETED_OVERFLOW:
            return FSHP_PARSER_STATUS_INTERPRETED_OVERFLOW;
        case PARSER_STATUS_COMPILED_OVERFLOW:
            return FSHP_PARSER_STATUS_COMPILED_OVERFLOW;
        case PARSER_STATUS_INVALID_BASE:
            return FSHP_PARSER_STATUS_INVALID_BASE;
        case PARSER_STATUS_INVALID_BASE10:
            return FSHP_PARSER_STATUS_INVALID_BASE10;
        case PARSER_STATUS_INVALID_BASE16:
            return FSHP_PARSER_STATUS_INVALID_BASE16;
        case PARSER_STATUS_NESTED_COMPILATION_MODE:
            return FSHP_PARSER_STATUS_NESTED_COMPILATION_MODE;
        case PARSER_STATUS_NOT_IN_COMPILATION_MODE:
            return FSHP_PARSER_STATUS_NOT_IN_COMPILATION_MODE;
        case PARSER_STATUS_NOT_IN_INTERPRETATION_MODE:
            return FSHP_PARSER_STATUS_NOT_IN_INTERPRETATION_MODE;
        case PARSER_STATUS_LOOKUP_OVERFLOW:
            return FSHP_PARSER_STATUS_LOOKUP_OVERFLOW;
        case PARSER_STATUS_PENDING_DEFINITION:
            return FSHP_PARSER_STATUS_PENDING_DEFINITION;
        case PARSER_STATUS_PARSER_CONTROL_STACK_OVERFLOW:
            return FSHP_PARSER_STATUS_PARSER_CONTROL_STACK_OVERFLOW;
        case PARSER_STATUS_PARSER_CONTROL_STACK_UNDERFLOW:
            return FSHP_PARSER_STATUS_PARSER_CONTROL_STACK_UNDERFLOW;
        case PARSER_STATUS_PARSER_CONTROL_STACK_MISMATCH:
            return FSHP_PARSER_STATUS_PARSER_CONTROL_STACK_MISMATCH;
        case PARSER_STATUS_INCOMPLETE_STRING:
            return FSHP_PARSER_STATUS_INCOMPLETE_STRING;
        case PARSER_STATUS_LITERAL_OVERFLOW:
            return FSHP_PARSER_STATUS_LITERAL_OVERFLOW;
        case PARSER_STATUS_COUNTED_STRING_OVERFLOW:
            return FSHP_PARSER_STATUS_COUNTED_STRING_OVERFLOW;
        case PARSER_STATUS_MODULE_NAME_OVERFLOW:
            return FSHP_PARSER_STATUS_MODULE_NAME_OVERFLOW;
        case PARSER_STATUS_MOD_OVERFLOW:
            return FSHP_PARSER_STATUS_MOD_OVERFLOW;
        case PARSER_STATUS_MOD_LOAD:
            return FSHP_PARSER_STATUS_MOD_LOAD;
        default:
            break;
        }
    }
    vm_reset(vm);
    ForthVMStatus ret_vm = vm_run(vm, false);
    if (ret_vm) {
        switch (ret_vm) {
        case VM_STATUS_OUT_OF_MEMORY:
            return FSHP_VM_STATUS_OUT_OF_MEMORY;
        case VM_STATUS_DATA_STACK_OVERFLOW:
            return FSHP_VM_STATUS_DATA_STACK_OVERFLOW;
        case VM_STATUS_DATA_STACK_UNDERFLOW:
            return FSHP_VM_STATUS_DATA_STACK_UNDERFLOW;
        case VM_STATUS_INVALID_BASE:
            return FSHP_VM_STATUS_INVALID_BASE;
        case VM_STATUS_CONTROL_STACK_OVERFLOW:
            return FSHP_VM_STATUS_CONTROL_STACK_OVERFLOW;
        case VM_STATUS_CONTROL_STACK_UNDERFLOW:
            return FSHP_VM_STATUS_CONTROL_STACK_UNDERFLOW;
        case VM_STATUS_INVALID_RETURN_ADDRESS:
            return FSHP_VM_STATUS_INVALID_RETURN_ADDRESS;
        case VM_STATUS_INVALID_MEMORY_ADDRESS:
            return FSHP_VM_STATUS_INVALID_MEMORY_ADDRESS;
        case VM_STATUS_INVALID_MEMORY_ALLOCATION:
            return FSHP_VM_STATUS_INVALID_MEMORY_ALLOCATION;
        case VM_STATUS_FLOAT_STACK_OVERFLOW:
            return FSHP_VM_STATUS_FLOAT_STACK_OVERFLOW;
        case VM_STATUS_FLOAT_STACK_UNDERFLOW:
            return FSHP_VM_STATUS_FLOAT_STACK_UNDERFLOW;
        default:
            break;
        }
    }
    return FSHP_STATUS_OK;
}

FSHPStatus fshp_process_file(
    char *fn, ForthVM *vm, ForthParser *parser, String *buf) {
    if (!vm || !parser) {
        return FSHP_STATUS_OK;
    }
    DArrayChar_clear(buf);
    FILE *fin = fopen(fn, "r");
    size_t idx = 0;
    FSHPStatus status = FSHP_STATUS_OK;
    for (char i = fgetc(fin); !feof(fin); i = fgetc(fin)) {
        int ret = DArrayChar_push_back(buf, &i);
        if (ret) {
            fclose(fin);
            return FSHP_STATUS_OUT_OF_MEMORY;
        }
        if (buf->size > 5) {
            if (!memcmp(buf->data + buf->size - 6, "<?fshp", 6)) {
                status = run_code(fin, parser, vm, buf, idx + 6);
                if (status) {
                    break;
                }
                idx = buf->size;
                if (feof(fin)) {
                    break;
                }
            } else {
                putchar(buf->data[idx++]);
            }
        }
    }
    if (!status) {
        fwrite(buf->data + idx, 1, buf->size - idx, stdout);
    }
    fclose(fin);
    return status;
}
