#include "mheads.h"
#include "lheads.h"
#include "ospd.h"

NEOERR* spd_pre_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "member");

    if (!cgi || !cgi->hdf || !evt) return nerr_raise(NERR_ASSERT, "paramter null");

    hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_SPD_PEEL, FLAGS_SYNC);

    hdf_copy(cgi->hdf, PRE_OUTPUT, evt->hdfrcv);

    return STATUS_OK;
}

NEOERR* spd_do_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt;
    int cityid = 0;
    
    if (!cgi || !cgi->hdf) return nerr_raise(NERR_ASSERT, "paramter null");

    /*
     * member
     */
    char *s = hdf_get_value(cgi->hdf, PRE_QUERY".city", NULL);
    if (s) {
        evt = hash_lookup(evth, "city");
        if (!evt) return nerr_raise(NERR_ASSERT, "city null");

        hdf_set_value(evt->hdfsnd, "c", s);
        MEVENT_TRIGGER_NRET(evt, NULL, REQ_CMD_CITY_BY_S, FLAGS_SYNC);
        cityid = hdf_get_int_value(evt->hdfrcv, "city.id", 0);
    }

#if 0
    s = hdf_get_value(cgi->hdf, PRE_QUERY".mname", NULL);
    if (s && !reg_search(REXP_EMAIL, s))
        return nerr_raise(LERR_USERINPUT, "%s not email", s);
#endif
    
    evt = hash_lookup(evth, "member");
    if (!evt) return nerr_raise(NERR_ASSERT, "member null");

    hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));

    hdf_set_int_value(evt->hdfsnd, "cityid", cityid);
    hdf_set_value(evt->hdfsnd, "mstatu", "10");
    if (hdf_get_int_value(evt->hdfsnd, "dad", 0) == 1)
        hdf_set_value(evt->hdfsnd, "_addcar", "1");
    
    MEVENT_TRIGGER(evt, NULL, REQ_CMD_MEMBER_ADD, FLAGS_SYNC);

    /*
     * plan
     */
    evt = hash_lookup(evth, "plan");
    if (!evt) return nerr_raise(NERR_ASSERT, "plan backend error");
    
    hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));
    hdf_set_value(evt->hdfsnd, "pstatu", "10");

    s = hdf_get_value(evt->hdfsnd, "stime", NULL);
    if (s && !reg_search("^[0-2][0-9](:[0-9][0-9])+$", s))
        hdf_remove_tree(evt->hdfsnd, "stime");

    s = hdf_get_value(evt->hdfsnd, "etime", NULL);
    if (s && !reg_search("^[0-2][0-9](:[0-9][0-9])+$", s))
        hdf_remove_tree(evt->hdfsnd, "etime");

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_PLAN_ADD, FLAGS_NONE);

    return STATUS_OK;
}

NEOERR* spd_post_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mdb_conn *db = hash_lookup(dbh, "plan");
	NEOERR *err;

    if (!cgi || !cgi->hdf || !db) return nerr_raise(NERR_ASSERT, "paramter null");

    MDB_QUERY_RAW(db, "plan", _COL_PLAN, "pstatu=%d LIMIT 1",
                  NULL, PLAN_ST_SPD_FRESH);
    err = mdb_set_row(cgi->hdf, db, _COL_PLAN, PRE_OUTPUT".plan");
	if (err != STATUS_OK) return nerr_pass(err);

    db = hash_lookup(dbh, "member");
    if (!db) return nerr_raise(NERR_ASSERT, "member null");

    int mid = hdf_get_int_value(cgi->hdf, PRE_OUTPUT".plan.mid", 0);

    MDB_QUERY_RAW(db, "member", _COL_MEMBER, "mid=%d", NULL, mid);
    err = mdb_set_row(cgi->hdf, db, _COL_MEMBER, PRE_OUTPUT".plan");
    if (err != STATUS_OK) return nerr_pass(err);

    int cityid = hdf_get_int_value(cgi->hdf, PRE_OUTPUT".plan.cityid", 0);

    MDB_QUERY_RAW(db, "city", _COL_CITY, "id=%d", NULL, cityid);
    mdb_set_row(cgi->hdf, db, _COL_CITY, PRE_OUTPUT".plan");
    nerr_ignore(&err);
    
    return STATUS_OK;
}

NEOERR* spd_post_data_mod(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt;
    char *sc, *ec;
    int scid = 0, ecid = 0;

    if (!cgi || !cgi->hdf) return nerr_raise(NERR_ASSERT, "paramter null");

    sc = hdf_get_value(cgi->hdf, PRE_QUERY".plan.scity", NULL);
    ec = hdf_get_value(cgi->hdf, PRE_QUERY".plan.ecity", NULL);
    if (sc || ec) {
        evt = hash_lookup(evth, "city");
        if (!evt) return nerr_raise(NERR_ASSERT, "city null");

        if (sc) hdf_set_value(evt->hdfsnd, "c.0", sc);
        if (ec) hdf_set_value(evt->hdfsnd, "c.1", ec);

        MEVENT_TRIGGER_NRET(evt, NULL, REQ_CMD_CITY_BY_S, FLAGS_SYNC);

        if (sc) {
            sc = hdf_get_valuef(evt->hdfrcv, "city.%s.id", sc);
            scid = atoi(sc);
        }
        if (ec) {
            ec = hdf_get_valuef(evt->hdfrcv, "city.%s.id", ec);
            ecid = atoi(ec);
        }
    }
    
    evt = hash_lookup(evth, "plan");
    if (!evt) return nerr_raise(NERR_ASSERT, "plan null");
    
    if (!hdf_get_obj(cgi->hdf, PRE_QUERY".plan"))
        return nerr_raise(NERR_ASSERT, "plan null");
    
    hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY".plan"));
    if (sc) hdf_set_int_value(evt->hdfsnd, "scityid", scid);
    if (ec )hdf_set_int_value(evt->hdfsnd, "ecityid", ecid);

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_PLAN_UP, FLAGS_NONE);

    return STATUS_OK;
}
