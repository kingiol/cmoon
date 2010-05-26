#!/bin/bash

sqlite3 prddb "
CREATE TABLE feedback (
id INTEGER PRIMARY KEY AUTOINCREMENT,
contact TEXT NOT NULL DEFAULT '',
content TEXT NOT NULL DEFAULT '');"
