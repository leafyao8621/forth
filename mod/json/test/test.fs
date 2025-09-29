mload" json"
mload" stdio"

json.document.create document
create str
    char { c, char " c, char a c, char " c, char : c, char 1 c, char , c,
    char " c, char b c, char " c, char : c, char t c, char r c, char u c,
    char e c, char , c, char " c, char c c, char " c, char : c, char " c,
    char a c, char b c, char c c, char " c, char , c, char " c, char d c,
    char " c, char : c, char [ c, char 1 c, char , c, char t c, char r c,
    char u c, char e c, char , c, char " c, char a c, char b c, char c c,
    char " c, char , c, char { c, char " c, char a c, char " c, char : c,
    char 1 c, char , c, char " c, char b c, char " c, char : c, char t c,
    char r c, char u c, char e c, char } c, char , c, char   c, char n c,
    char u c, char l c, char l c, char ] c, char , c, char " c, char e c,
    char " c, char : c, char { c, char " c, char a c, char " c, char : c,
    char 1 c, char , c, char " c, char b c, char " c, char : c, char t c,
    char r c, char u c, char e c, char , c, char " c, char c c, char " c,
    char : c, char " c, char a c, char b c, char c c, char " c, char , c,
    char " c, char d c, char " c, char : c, char [ c, char 1 c, char , c,
    char t c, char r c, char u c, char e c, char , c, char " c, char a c,
    char b c, char c c, char " c, char , c, char { c, char " c, char a c,
    char " c, char : c, char 1 c, char , c, char " c, char b c, char " c,
    char : c, char t c, char r c, char u c, char e c, char } c, char , c,
    char   c, char n c, char u c, char l c, char l c, char ] c, char , c,
    char " c, char e c, char " c, char : c, char { c, char " c, char a c,
    char " c, char : c, char 1 c, char , c, char " c, char b c, char " c,
    char : c, char t c, char r c, char u c, char e c, char } c, char , c,
    char   c, char " c, char f c, char " c, char : c, char   c, char n c,
    char u c, char l c, char l c, char } c, char , c, char   c, char " c,
    char f c, char " c, char : c, char   c, char n c, char u c, char l c,
    char l c, char } c, 0 c,

: print.type
    case
        JSON.NODE.TYPE.BOOLEAN of
            ." (boolean)"
        endof
        JSON.NODE.TYPE.NUMBER of
            ." (number)"
        endof
        JSON.NODE.TYPE.STRING of
            ." (string)"
        endof
        JSON.NODE.TYPE.ARRAY of
            ." (array)"
        endof
        JSON.NODE.TYPE.OBJECT of
            ." (object)"
        endof
    endcase
;

: pprint
    ." Node: "
    dup z" 0x%016X" swap printf.d cr
    dup json.node.get.is.null if
        ." null" cr
        drop
        exit
    then
    dup json.node.get.type dup . 32 emit print.type cr
    case
        JSON.NODE.TYPE.NUMBER of
            drop
            dup json.node.get.number f. cr
        endof
        JSON.NODE.TYPE.BOOLEAN of
            drop
            dup json.node.get.boolean if
                ." true" cr
            else
                ." false" cr
            then
        endof
        JSON.NODE.TYPE.STRING of
            drop
            dup json.node.get.string puts
        endof
        JSON.NODE.TYPE.ARRAY of
            drop
            ." Length: "
            dup json.node.get.length dup . cr
            0 do
                ." Index: " i . cr
                dup i json.node.get.node.by.index recurse
            loop
        endof
        JSON.NODE.TYPE.OBJECT of
            drop
            ." Length: "
            dup json.node.get.length dup . cr
            0 do
                dup i json.node.get.pair.by.index
                dup json.pair.get.in.use if
                    ." Index: " i . cr
                    ." Key: "
                    dup json.pair.get.key puts
                    ." Value: " cr
                    dup json.pair.get.node recurse
                then
                drop
            loop
        endof
    endcase
    drop
;

: main
    str puts
    document str json.document.parse
    . cr
    document json.document.get.root
    dup pprint
    drop
    document json.document.finalize
    . cr
;
