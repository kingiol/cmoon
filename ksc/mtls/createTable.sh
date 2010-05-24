#!/bin/bash

sqlite3 prddb "
CREATE TABLE prd (
id INTEGER PRIMARY KEY AUTOINCREMENT,
type INTEGER NOT NULL DEFAULT 0,
tid INTEGER NOT NULL DEFAULT 0,
name TEXT NOT NULL DEFAULT '',
url TEXT NOT NULL DEFAULT '',
des TEXT NOT NULL DEFAULT '',
intime TEXT NOT NULL DEFAULT '2009-01-01 00:00:01');"

sqlite3 prddb 'CREATE INDEX indprd ON prd (type, tid);'

sqlite3 prddb "INSERT INTO prd (name) VALUES ('Category_1');"
sqlite3 prddb "INSERT INTO prd (name) VALUES ('Category_2');"
sqlite3 prddb "INSERT INTO prd (name) VALUES ('Category_3');"
sqlite3 prddb "INSERT INTO prd (name) VALUES ('Category_4');"
sqlite3 prddb "INSERT INTO prd (name) VALUES ('Category_5');"
sqlite3 prddb "INSERT INTO prd (name) VALUES ('Category_6');"
sqlite3 prddb "INSERT INTO prd (name) VALUES ('Category_7');"
sqlite3 prddb "INSERT INTO prd (name) VALUES ('Category_8');"
sqlite3 prddb "INSERT INTO prd (name) VALUES ('Category_9');"
sqlite3 prddb "INSERT INTO prd (name) VALUES ('Category_10');"
sqlite3 prddb "INSERT INTO prd (name) VALUES ('Category_11');"
sqlite3 prddb "INSERT INTO prd (name) VALUES ('Category_12');"

sqlite3 prddb "
CREATE TABLE user (
uname TEXT NOT NULL DEFAULT '',
usn TEXT NOT NULL DEFAULT '');"

sqlite3 prddb "INSERT INTO user (uname, usn) VALUES ('mandy', '1284f7cd4392e1ebd58dda15e2ef7bb1');"
