#!/bin/bash
rm parser_handler.c
for i in `ls src/parser/handler/*.c`; do
    sed /#include/d $i >> parser_handler.c
done

rm vm_handler.c
for i in `ls src/vm/handler/*.c`; do
    sed /#include/d $i >> vm_handler.c
done
