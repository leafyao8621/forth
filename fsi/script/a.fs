create a 4 allot
: main
    a
    4 0 do
        dup
        65 i + swap c!
        1+
    loop
    drop
    a
    4 0 do
        dup c@ emit cr
        1+
    loop
    drop
;
