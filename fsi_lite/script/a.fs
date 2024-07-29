: test if 65 emit else 66 emit then ;
: test-loop 0 do i . cr i test cr loop ;
: test-+loop 0 do i . cr i test cr 10 +loop ;
: test--loop 0 do i . cr i test cr -1 +loop ;
: main 10 test-loop 100 test-+loop -10 test--loop ;
