1 2 3
0 pick .
1 pick .
2 pick .
cr drop drop drop

: func
    4 1 do
        i
    loop
    3 0 do
        i pick .
    loop
    cr
    3 0 do
        drop
    loop
;

func
