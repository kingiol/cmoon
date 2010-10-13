#!/bin/sh

psql -U lcser lcs_mtool -c "INSERT INTO stat (type, count) SELECT 0, * FROM count_app_yesterday;"
psql -U lcser lcs_mtool -c "INSERT INTO stat (type, count) SELECT 1, * FROM count_user_yesterday;"
psql -U lcser lcs_mtool -c "INSERT INTO stat (type, count) SELECT 2, * FROM count_join_yesterday;"
psql -U lcser lcs_mtool -c "INSERT INTO stat (type, count) SELECT 3, * FROM count_visit_yesterday;"
psql -U lcser lcs_mtool -c "INSERT INTO stat (type, count) SELECT 4, * FROM count_msg_yesterday;"


psql -U lcser lcs_mtool -c "INSERT INTO stat (type, count) SELECT 100, * FROM count_app_total;"
psql -U lcser lcs_mtool -c "INSERT INTO stat (type, count) SELECT 101, * FROM count_user_total;"
psql -U lcser lcs_mtool -c "INSERT INTO stat (type, count) SELECT 102, * FROM count_join_total;"
psql -U lcser lcs_mtool -c "INSERT INTO stat (type, count) SELECT 103, * FROM count_visit_total;"
psql -U lcser lcs_mtool -c "INSERT INTO stat (type, count) SELECT 104, * FROM count_msg_total;"
