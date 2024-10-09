: point create 2 cells allot ;
: point-x ;
: point-y 1 cells + ;
: point-print dup point-x @ . 32 emit point-y @ . cr ;

point pa
10 pa point-x !
20 pa point-y !

point pb
30 pb point-x !
40 pb point-y !

pa point-print
pb point-print
