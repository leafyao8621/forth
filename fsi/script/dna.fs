create lookup 65 c, 84 c, 71 c, 67 c,
: dna
    0 do
        i
        32 0 do
            dup 3 and lookup + c@ emit
            2 rshift
        loop
        cr
        drop
    loop
;

500000 dna
