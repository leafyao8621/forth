variable a
variable b

: main
    b !
    a !
;

12345 -1 main
a @ . cr
b @ . cr

: inc dup @ 1+ swap ! ;

a inc
a @ . cr
b inc
b @ . cr
