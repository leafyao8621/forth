: main
    -1E d>f
    100 0 do
        fdup f.
        space
        fdup fasin f.
        space
        fdup facos f.
        space
        fdup fatan f.
        cr
        0.02 d>f f+
    loop
    fdrop
;
