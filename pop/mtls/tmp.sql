\c lcs_dyn;

DROP TABLE lcsjoin;
DROP TABLE visit;

CREATE TABLE track (
	   id SERIAL,
	   type int NOT NULL DEFAULT 0, -- 0: join 1: visit
	   aname varchar(256) NOT NULL DEFAULT '',
	   uname varchar(256) NOT NULL DEFAULT '',
	   oname varchar(256) NOT NULL DEFAULT '',
	   aid int NOT NULL DEFAULT 0,
	   uid int NOT NULL DEFAULT 0,
	   oid int NOT NULL DEFAULT 0,
	   ip varchar(32) NOT NULL DEFAULT '',
	   area varchar(64) NOT NULL DEFAULT '',
	   url varchar(256) NOT NULL DEFAULT '',
	   title varchar(256) NOT NULL DEFAULT '',
	   refer varchar(256) NOT NULL DEFAULT '',
	   intime timestamp DEFAULT now(),
	   PRIMARY KEY (id)
);

CREATE INDEX track_index ON track (uid, oid);

\c lcs_main;

ALTER TABLE userinfo ADD addr varchar(64) NOT NULL DEFAULT '';

\c lcs_mtool;

ALTER TABLE stat RENAME TO kol;
ALTER SEQUENCE stat_id_seq RENAME TO kol_id_seq;
CREATE TABLE visit (
	   id SERIAL,
	   aid int NOT NULL DEFAULT 0,
	   pv int NOT NULL DEFAULT 0,
	   uv int NOT NULL DEFAULT 0,
	   dt date DEFAULT current_date,
	   PRIMARY KEY (id)
);

CREATE TABLE topref (
	   id SERIAL,
	   aid int NOT NULL DEFAULT 0,
	   refer varchar(256) NOT NULL DEFAULT 0,
	   count int NOT NULL DEFAULT 0,
	   dt date DEFAULT current_date,
	   PRIMARY KEY (id)
);

CREATE TABLE topurl (
	   id SERIAL,
	   aid int NOT NULL DEFAULT 0,
	   url varchar(256) NOT NULL DEFAULT 0,
	   title varchar(256) NOT NULL DEFAULT 0,
	   count int NOT NULL DEFAULT 0,
	   dt date DEFAULT current_date,
	   PRIMARY KEY (id)
);

CREATE TABLE toparea (
	   id SERIAL,
	   aid int NOT NULL DEFAULT 0,
	   area varchar(64) NOT NULL DEFAULT '',
	   count int NOT NULL DEFAULT 0,
	   dt date DEFAULT current_date,
	   PRIMARY KEY (id)
);

\i view_mtls.sql;
