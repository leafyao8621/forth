: cd
    dup 0= if
        exit
    then
    dup . cr
    1-
    recurse
;
