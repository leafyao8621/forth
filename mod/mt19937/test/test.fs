mload" mt19937"

mt19937.create gen

: main
    gen 1000 mt19937.initialize
    10 0 do
        gen mt19937.gen . cr
    loop
;
