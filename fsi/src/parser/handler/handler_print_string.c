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
    if (!parser->compile) {
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
    }
    ret =
        DArrayChar_push_back_batch(
            &vm->literal, parser->token_buf.data, parser->token_buf.size);
    if (ret) {
        return FORTHVM_ERR_OUT_OF_MEMORY;
    }
    return FORTHVM_ERR_OK;
}
