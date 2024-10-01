create a 10 , 20 , 30 , 40 ,
create b 65 c, 66 c, 67 c, 68 c,
: main
    a
    4 0 do
        dup @ . cr
        1 cells +
    loop
    drop
    b
    4 0 do
        dup c@ emit cr
        1+
    loop
    drop
;
main
