: cond
    dup 90 >= if
        ." A" cr
    else
        dup 80 >= if
            ." B" cr
        else
            dup 70 >= if
                ." C" cr
            else
                dup 60 >= if
                    ." D" cr
                else
                    ." F" cr
                then
            then
        then
    then
;

95 cond . cr
90 cond . cr
85 cond . cr
80 cond . cr
75 cond . cr
70 cond . cr
65 cond . cr
60 cond . cr
55 cond . cr

2 3 swap . .
5 6 drop . cr
