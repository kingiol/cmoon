#include "mheads.h"
#include "lheads.h"
#include "obore.h"

NEOERR* bore_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	NEOERR *err;
    mdb_conn *db = hash_lookup(dbh, "aux");

    MCS_NOT_NULLB(cgi->hdf, db);

    MDB_QUERY_RAW(db, "memory", _COL_MEMORY, "statu=%d ORDER BY random() LIMIT 1",
                  NULL, MEMORY_ST_OK);
    err = mdb_set_row(cgi->hdf, db, _COL_MEMORY, PRE_OUTPUT".memory");

    return nerr_pass(err);
}

NEOERR* bore_op_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "member");
    mdb_conn *db = hash_lookup(dbh, "aux");
    char *mname;
	NEOERR *err;
    
    MEMBER_CHECK_ADMIN();

    int id = hdf_get_int_value(cgi->hdf, PRE_QUERY".id", 0);
    if (id <= 0) return STATUS_OK;

    MDB_QUERY_RAW(db, "memory", _COL_MEMORY, "id=%d", NULL, id);
    return nerr_pass(mdb_set_row(cgi->hdf, db, _COL_MEMORY, PRE_OUTPUT".memory"));
}

NEOERR* bore_op_data_mod(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "member");
    mdb_conn *db = hash_lookup(dbh, "aux");
	STRING str; string_init(&str);
    char *mname;
    int id;
	NEOERR *err;

    MEMBER_CHECK_ADMIN();

    HDF_GET_INT(cgi->hdf, PRE_QUERY".id", id);

    err = mdb_build_upcol(hdf_get_obj(cgi->hdf, PRE_QUERY),
                          hdf_get_obj(g_cfg, "Db.UpdateCol.memory"), &str);
	if (err != STATUS_OK) return nerr_pass(err);

    MDB_EXEC(db, NULL, "UPDATE memory SET %s WHERE id=%d;", NULL, str.buf, id);

    string_clear(&str);

    return STATUS_OK;
}

NEOERR* bore_op_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "member");
    mdb_conn *db = hash_lookup(dbh, "aux");
	STRING str; string_init(&str);
    char *mname;
	NEOERR *err;

    MEMBER_CHECK_ADMIN();

    err = mdb_build_incol(hdf_get_obj(cgi->hdf, PRE_QUERY),
                          hdf_get_obj(g_cfg, "Db.InsertCol.memory"), &str);
	if (err != STATUS_OK) return nerr_pass(err);

    MDB_EXEC(db, NULL, "INSERT into memory %s", NULL, str.buf);

    string_clear(&str);

    return STATUS_OK;
}

NEOERR* bore_preview_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{

    MCS_NOT_NULLA(cgi->hdf);

    hdf_set_copy(cgi->hdf, PRE_OUTPUT".s", PRE_QUERY".s");
    
    return STATUS_OK;
}
