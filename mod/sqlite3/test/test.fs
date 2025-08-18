mload" sqlite3"
mload" stdio"

variable conn
variable stmt

: insert
    ." Insert" cr
    conn @
    z" INSERT INTO TEST (C_INT, C_FLOAT, C_TXT) VALUES (?, ?, ?)"
    stmt
    sqlite3.prepare
    drop
    0 d>f
    100 0 do
        ." Iter " i . cr
        stmt @ 1 i 1+ 2* sqlite3.bind.int
        drop
        stmt @ 2 fdup 1.5 d>f f* sqlite3.bind.float
        1E d>f f+
        drop
        stmt @ 3
        i 1 and if
            z" text" sqlite3.bind.text
        else
            sqlite3.bind.null
        then
        drop
        stmt @ sqlite3.step
        drop
        stmt @ sqlite3.reset
        drop
    loop
    fdrop
    stmt @ sqlite3.finalize
    drop
;

: read
    ." Read" cr
    conn @
    z" SELECT * FROM TEST"
    stmt
    sqlite3.prepare
    begin stmt @ sqlite3.step SQLITE.DONE = invert while
        stmt @ 0 sqlite3.column.int . ."  | "
        stmt @ 1 sqlite3.column.int . ."  | "
        stmt @ 2 sqlite3.column.float f. ."  | "
        stmt @ 3 sqlite3.column.text
        dup if
            puts
        else
            drop ." NULL" cr
        then
    repeat
    stmt @ sqlite3.finalize
    drop
;

: main
    z" data/test.db" conn sqlite3.open
    drop
    conn @
    z" DROP TABLE IF EXISTS TEST"
    stmt
    sqlite3.prepare
    drop
    stmt @ sqlite3.step
    drop
    stmt @ sqlite3.finalize
    drop
    conn @
    z" CREATE TABLE IF NOT EXISTS TEST (ID INTEGER PRIMARY KEY AUTOINCREMENT, C_INT INTEGER, C_FLOAT REAL, C_TXT TEXT)"
    stmt
    sqlite3.prepare
    drop
    stmt @ sqlite3.step
    drop
    stmt @ sqlite3.finalize
    drop
    insert
    read
    conn @ sqlite3.close
    drop
;
