mload" json"
mload" stdio"

json.document.create document
create str
    char { c,
    char " c, char a c, char " c, char : c, char 1 c, char , c,
    char " c, char b c, char " c, char : c, char t c, char r c,
    char u c, char e c,
    char } c, 0 c,

: main
    str puts
    document str json.document.parse
    . cr
    document json.document.get.root
    z" 0x%016X" swap printf.d cr
    document json.document.finalize
    . cr
;
