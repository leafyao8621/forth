: mkctr variable does> dup dup @ 1+ swap ! ;
mkctr if
: asdf ;
mkctr asdf
mkctr xxxx
: ctrif 0 if ! 10 0 do if @ . cr loop ;
: ctrasdf 0 asdf ! 10 0 do asdf @ . cr loop ;
: ctrxxxx 0 xxxx ! 10 0 do xxxx @ . cr loop ;
mkctr foo
mkctr foo
