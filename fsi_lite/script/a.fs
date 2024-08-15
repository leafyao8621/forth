: test-until
    0
    begin
        dup . cr
        1+
        dup 9 >
    until
    drop
;
: test-again
    0
    begin
        dup . cr
        1+
        dup 9 > if
            drop
            exit
        then
    again
;
: main
    test-until
    test-again
;
