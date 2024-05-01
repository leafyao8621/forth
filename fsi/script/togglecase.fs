: togglecase
    begin
        dup dup c@ dup
    while
        32 xor swap c!
        1+
    repeat
    drop
;

: pns
    begin
        dup c@ dup
    while
        emit 1+
    repeat
    cr
;

create str 65 c, 66 c, 67 c, 68 c, 0 c,

: main
    ." init" cr
    str pns
    4 0 do
        i . cr
        str togglecase
        str pns
    loop
;

main
