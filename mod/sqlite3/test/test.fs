mload" sqlite3"
variable conn

: main
    z" data/test.db" conn sqlite3.open
    conn @ sqlite3.close
;
