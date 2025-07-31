mload" stdio"

: main
    z" 0x%016lx" 65535 printf.d
    cr
    z" %010.2lf" 65535.456 d>f printf.f
    cr
    z" dump" z" w" fopen
    dup
    z" 0x%016lx" 65535 fprintf.d
    dup
    z" %010.2lf" 65535.456 d>f fprintf.f
    fclose
    stdout z" 0x%016lx" 65535 fprintf.d
    cr
    stdout z" %010.2lf" 65535.456 d>f fprintf.f
    cr
;
