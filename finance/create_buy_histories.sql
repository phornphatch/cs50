CREATE TABLE
    buy_histories (
        id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
        symbol TEXT,
        name TEXT,
        shares NUMERIC,
        price NUMERIC,
        total NUMERIC,
        user_id INTEGER,
        FOREIGN KEY (user_id) REFERENCES users(id)
    );