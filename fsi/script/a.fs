variable a
variable b
a . cr
b . cr
variable a
a . cr
create a
a . cr
create b
b . cr
1 cells allot
10 a !
a @ . cr
b @ . cr
20 b !
a @ . cr
b @ . cr
create a 1 cells allot
create b 1 cells allot
50 a !
a @ . cr
b @ . cr
100 a 1 cells + !
b @ . cr

: setup
    a
    1 cells 0 do
        dup i swap c!
        1+
    loop
;

: retrieve
    a
    1 cells 0 do
        dup c@ . cr
        1+
    loop
;

setup
retrieve
