create arr 10 cells allot
: main
    arr
    10 0 do
        dup i swap !
        cell+
    loop
    drop
    arr
    10 0 do
        dup @ . cr
        cell+
    loop
    drop
;
