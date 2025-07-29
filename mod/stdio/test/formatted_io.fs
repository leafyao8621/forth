mload" stdio"

: main
    z" %s" z" abcd" puts
    z" 0x%016lx" 65535 printf.d
    cr
    z" %010.2lf" 65535.456 d>f printf.f
    cr
;
