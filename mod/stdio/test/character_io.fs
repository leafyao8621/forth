mload" stdio"

: main
    z" abcd" puts
    char A putchar
    10 putchar
    z" dump" z" w" fopen
    dup
    z" blah" swap fputs
    dup
    char A swap fputc
    depth . cr
    fclose
;
