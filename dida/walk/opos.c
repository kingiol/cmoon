#include "mheads.h"
#include "lheads.h"
#include "opos.h"

NEOERR* pos_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mdb_conn *db = hash_lookup(dbh, "city");
	NEOERR *err;

    if (!cgi || !cgi->hdf || !db) return nerr_raise(NERR_ASSERT, "paramter null");

    MDB_QUERY_RAW(db, "city", _COL_CITY, "geopos ?- point '(0,0)' LIMIT 1000", NULL);
    return nerr_pass(mdb_set_rows(cgi->hdf, db, _COL_CITY, PRE_OUTPUT".cts", NULL));
}

NEOERR* pos_data_mod(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	STRING str; string_init(&str);
    mdb_conn *db = hash_lookup(dbh, "city");
	NEOERR *err;

    if (!cgi || !cgi->hdf || !db) return nerr_raise(NERR_ASSERT, "paramter null");

    int id = hdf_get_int_value(cgi->hdf, PRE_QUERY".id", 0);
    
    err = mcs_build_upcol(hdf_get_obj(cgi->hdf, PRE_QUERY),
                          hdf_get_obj(g_cfg, "Db.UpdateCol.city"), &str);
	if (err != STATUS_OK) return nerr_pass(err);

    MDB_EXEC(db, NULL, "UPDATE city SET %s WHERE id=%d;", NULL, str.buf, id);
    

    return STATUS_OK;
}
