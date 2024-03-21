#include <fsi/util/parser_handlers.h>
#include <fsi/util/parser_operations.h>

ForthVMErr parser_handle_print_string(ForthParser *parser, ForthVM *vm) {
    if (!parser || !vm) {
        return FORTHVM_ERR_NULL_PTR;
    }
    ++parser->iter;
    DArrayChar_clear(&parser->token_buf);
    int ret = get_literal(parser);
    switch (ret) {
    case 1:
        return FORTHVM_ERR_OUT_OF_MEMORY;
    case 2:
        return FORTHVM_ERR_QUOTE;
    }
    char opcode = OPCODE_PRINT_STRING;
    size_t literal_offset = vm->literal.size;
    switch (parser->state) {
    case FORTHPARSER_STATE_INTERPRET:
        ret = DArrayChar_push_back(&vm->interpreted, &opcode);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        ret =
            DArrayChar_push_back_batch(
                &vm->interpreted, (char*)&literal_offset, sizeof(size_t));
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        break;
    case FORTHPARSER_STATE_COMPILE:
        ret = DArrayChar_push_back(&vm->compiled, &opcode);
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        ret =
            DArrayChar_push_back_batch(
                &vm->compiled, (char*)&literal_offset, sizeof(size_t));
        if (ret) {
            return FORTHVM_ERR_OUT_OF_MEMORY;
        }
        break;
    case FORTHPARSER_STATE_DEFINE:
        parser->offset = 0;
        vm->offset.data[0] = vm->compiled.size;
        vm->offset_flags.data[0] = OFFSET_PENDING;
        break;
    }
    ret =
        DArrayChar_push_back_batch(
            &vm->literal, parser->token_buf.data, parser->token_buf.size);
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    return FORTHVM_ERR_OK;
}
