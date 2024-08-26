create arr 2000 cells allocate
1 arr !
1 arr 1 cells + !
: fibb
    41 105 40 102 9 105
    6 0 do
        emit
    loop
    cr
    2 0 do
        i . 9 emit 1 . cr
    loop
    arr 2 cells +
    swap 2 do
        dup dup dup
        1 cells - @
        swap
        2 cells - @
        +
        i . 9 emit dup . cr
        swap
        !
        1 cells +
    loop
    drop
;
