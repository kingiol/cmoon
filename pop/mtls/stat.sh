#!/bin/sh

psql -U lcser lcs_mtool -c "INSERT INTO kol (type, count) SELECT 0, * FROM count_app_today;"
psql -U lcser lcs_mtool -c "INSERT INTO kol (type, count) SELECT 1, * FROM count_user_today;"
psql -U lcser lcs_mtool -c "INSERT INTO kol (type, count) SELECT 2, * FROM count_join_today;"
psql -U lcser lcs_mtool -c "INSERT INTO kol (type, count) SELECT 3, * FROM count_visit_today;"
psql -U lcser lcs_mtool -c "INSERT INTO kol (type, count) SELECT 4, * FROM count_msg_today;"
psql -U lcser lcs_mtool -c "INSERT INTO kol (type, count) SELECT 5, * FROM count_app_valid_today;"


psql -U lcser lcs_mtool -c "INSERT INTO kol (type, count) SELECT 100, * FROM count_app_total;"
psql -U lcser lcs_mtool -c "INSERT INTO kol (type, count) SELECT 101, * FROM count_user_total;"
psql -U lcser lcs_mtool -c "INSERT INTO kol (type, count) SELECT 102, * FROM count_join_total;"
psql -U lcser lcs_mtool -c "INSERT INTO kol (type, count) SELECT 103, * FROM count_visit_total;"
psql -U lcser lcs_mtool -c "INSERT INTO kol (type, count) SELECT 104, * FROM count_msg_total;"
psql -U lcser lcs_mtool -c "INSERT INTO kol (type, count) SELECT 105, * FROM count_app_valid_total;"


#/home/bigml/web/moon/pop/mtls/fill_trackarea

psql -U lcser lcs_mtool -c "INSERT INTO visit (aid, pv, uv) SELECT * FROM visit_today;"
psql -U lcser lcs_mtool -c "INSERT INTO topref (aid, refer, count) SELECT * FROM topref_today;"
psql -U lcser lcs_mtool -c "INSERT INTO topurl (aid, url, title, count) SELECT * FROM topurl_today;"
psql -U lcser lcs_mtool -c "INSERT INTO toparea (aid, area, count) SELECT * FROM toparea_today;"

/home/bigml/web/moon/pop/mtls/clearcache mtls
