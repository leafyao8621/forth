mload" httputil"
mload" stdio"

http.request.create request

: main
    test1 puts
    ." Initialize" cr
    request http.request.initialize
    ." Ret: " . cr
    ." Parse" cr
    request test1 http.request.parse
    ." Ret: " . cr
    ." Finalize" cr
    request http.request.finalize
    ." Ret: " . cr
;
