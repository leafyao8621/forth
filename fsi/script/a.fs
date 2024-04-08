: cond
    0 swap
    dup 3 mod 0 = if
        ." Fizz"
        swap 1+ swap
    then
    dup 5 mod 0 = if
        ." Buzz"
        swap 1+ swap
    then
    swap 0 = if
        .
    then
    cr
;

: main
    1 do
        i cond
    loop
;

16 main
