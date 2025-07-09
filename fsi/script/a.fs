: main
    -6.28 d>f
    100 0 do
        fdup f.
        space
        fdup fsinh f.
        space
        fdup fcosh f.
        space
        fdup ftanh f.
        cr
        0.1256 d>f f+
    loop
    fdrop
;
