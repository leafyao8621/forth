mload" sqlite3"
variable conn
variable stmt

: main
    z" data/test.db" conn sqlite3.open
    . cr
    conn @
    z" CREATE TABLE IF NOT EXISTS TEST (ID INT AUTO INCREMENT PRIMARY KEY, C_INT INT, C_FLOAT FLOAT, C_TXT TEXT)"
    stmt
    sqlite3.prepare
    . cr
    stmt @ sqlite3.step
    . cr
    stmt @ sqlite3.finalize
    . cr
    conn @ sqlite3.close
    . cr
;
