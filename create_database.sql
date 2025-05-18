CREATE TABLE user(
	email TEXT PRIMARY KEY,
	display_name TEXT NOT NULL,
	password TEXT NOT NULL
);
CREATE TABLE quest (
            id TEXT PRIMARY KEY,
	    owner_id TEXT REFERENCES user(email) NOT NULL ,	
            caption TEXT NOT NULL,
            parent_id TEXT,
            FOREIGN KEY (parent_id) REFERENCES quest(id)
);

