#include "mheads.h"
#include "lheads.h"
#include "obore.h"

NEOERR* bore_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	NEOERR *err;
    mdb_conn *db = hash_lookup(dbh, "aux");

    if (!cgi || !cgi->hdf || !db) return nerr_raise(NERR_ASSERT, "paramter null");

    MDB_QUERY_RAW(db, "memory", _COL_MEMORY, "statu=%d ORDER BY random() LIMIT 1",
                  NULL, MEMORY_ST_OK);
    err = mdb_set_row(cgi->hdf, db, _COL_MEMORY, PRE_OUTPUT".memory");

    return nerr_pass(err);
}

NEOERR* bore_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    if (!cgi || !cgi->hdf) return nerr_raise(NERR_ASSERT, "paramter null");

    return STATUS_OK;
}
