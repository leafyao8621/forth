create arr char A c, char T c, char G c, char C c,

: one
    0 do
        r> dup . cr >r
    loop
;

: two
    0 do
        2r> 2dup . space . cr 2>r
    loop
;
