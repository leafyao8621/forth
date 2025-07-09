: main
    -1E d>f
    100 0 do
        fdup f.
        space
        fdup fasinh f.
        space
        fdup 100E d>f f* fabs facosh f.
        space
        fdup fatanh f.
        cr
        0.02 d>f f+
    loop
    fdrop
;
