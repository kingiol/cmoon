#!/bin/bash

sqlite3 prddb "
CREATE TABLE user (
uname TEXT NOT NULL DEFAULT '',
usn TEXT NOT NULL DEFAULT '');"

sqlite3 prddb "INSERT INTO user (uname, usn) VALUES ('mandy', '1284f7cd4392e1ebd58dda15e2ef7bb1');"
