#!/bin/sh

psql -U lcser lcs_main -c "SELECT COUNT(*) FROM appinfo WHERE intime > current_date -1;"
