CREATE TABLE user(
	email TEXT PRIMARY KEY,
	display_name TEXT NOT NULL,
	password TEXT NOT NULL
);
CREATE TABLE quest (
    id TEXT PRIMARY KEY,
    status TEXT NOT NULL,
    title TEXT NOT NULL,
    caption TEXT NOT NULL,
    owner_id TEXT NOT NULL REFERENCES user(email),
    editor_id TEXT NOT NULL REFERENCES user(email),
    parent_id TEXT REFERENCES quest(id)
);

