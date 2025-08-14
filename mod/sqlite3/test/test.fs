mload" sqlite3"
variable conn
variable stmt.create

: main
    z" data/test.db" conn sqlite3.open
    . cr
    conn @
    z" CREATE TABLE IF NOT EXISTS TEST (ID INT AUTO INCREMENT PRIMARY KEY, C_INT INT, C_FLOAT FLOAT, C_TXT TEXT)"
    stmt.create
    sqlite3.prepare
    . cr
    stmt.create @ sqlite3.step
    . cr
    stmt.create @ sqlite3.finalize
    . cr
    conn @ sqlite3.close
    . cr
;
