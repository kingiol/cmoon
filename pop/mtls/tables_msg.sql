CREATE TABLE msg (
	   id SERIAL,
	   mfrom varchar(256) NOT NULL DEFAULT '',
	   mto varchar(256) NOT NULL DEFAULT '',
	   mtype int NOT NULL DEFAULT 0,
	   mraw varchar(1024) NOT NULL DEFAULT '',
	   intime timestamp DEFAULT now(),
	   PRIMARY KEY (id)
);
