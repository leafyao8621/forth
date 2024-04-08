: cond
    dup 90 >= if
        ." A "
    else
        dup 80 >= if
            ." B "
        else
            dup 70 >= if
                ." C "
            else
                dup 60 >= if
                    ." D "
                else
                    ." F "
                then
            then
        then
    then
;

: main
    1 do
        i cond . cr
    loop
;

100 main
