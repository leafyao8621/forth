: test
    dup 0= if
        drop
        exit
    then
    dup . cr
    1-
    dup
    recurse
    drop
;
