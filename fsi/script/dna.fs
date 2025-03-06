create arr char A c, char T c, char G c, char C c,

: main
    0 do
        3
        16 0 do
            dup j and i 2* rshift arr + c@ emit
            2 lshift
        loop
        drop
        cr
    loop
;
