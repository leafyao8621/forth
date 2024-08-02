: test
    if
        65 emit
    else
        66 emit
    then
;
: test-loop
    0 do
        i . cr
        i test cr
    loop
;
: test-+loop
    0 do
        i . cr
        i test cr
        10
    +loop
;
: test--loop
    0 do
        i . cr
        i test cr
        -1
    +loop
;
: test-loop-leave
    0 do
        i . cr
        i if
            leave
        then
    loop
    65 emit 66 emit cr
;
: test-+loop-leave
    0 do
        i . cr
        i if
            leave
        then
        10
    +loop
    65 emit 66 emit cr
;
: test-loop-exit
    0 do
        i . cr
        i if
            unloop exit
        then
    loop
    65 emit 66 emit cr
;
: test-+loop-exit
    0 do
        i . cr
        i if
            unloop exit
        then
        10
    +loop
    65 emit 66 emit cr
;
: main
    10 test-loop
    100 test-+loop
    -10 test--loop
    10 test-loop-leave
    100 test-+loop-leave
    10 test-loop-exit
    100 test-+loop-exit
;
