CREATE TABLE user(
	email TEXT PRIMARY KEY,
	display_name TEXT NOT NULL,
	password TEXT NOT NULL
);
CREATE TABLE quest (
        id TEXT PRIMARY KEY,
        caption TEXT NOT NULL,
        parent TEXT REFERENCES quest(id)
);
