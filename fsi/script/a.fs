: main
    -6.28 d>f
    100 0 do
        fdup f.
        space
        fdup fsin f.
        space
        fdup fcos f.
        space
        fdup ftan f.
        cr
        0.1256 d>f f+
    loop
    fdrop
;
