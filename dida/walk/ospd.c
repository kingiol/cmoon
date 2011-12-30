#include "mheads.h"
#include "lheads.h"
#include "ospd.h"

NEOERR* spd_pre_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "plan");

    if (!cgi || !cgi->hdf || !evt) return nerr_raise(NERR_ASSERT, "paramter null");

    hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_SPD_PEEL, FLAGS_SYNC);

    hdf_copy(cgi->hdf, PRE_OUTPUT, evt->hdfrcv);

    return STATUS_OK;
}

NEOERR* spd_do_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt;
    HDF *member, *plan;
    int cityid = 0;
    
    if (!cgi || !cgi->hdf) return nerr_raise(NERR_ASSERT, "paramter null");

    /*
     * member
     */
    HDF_GET_OBJ(cgi->hdf, PRE_QUERY".member", member);
    HDF_GET_OBJ(cgi->hdf, PRE_QUERY".plan", plan);
    
    char *s = hdf_get_value(member, "city", NULL);
    if (s) {
        evt = hash_lookup(evth, "city");
        if (!evt) return nerr_raise(NERR_ASSERT, "city null");

        hdf_set_value(evt->hdfsnd, "c", s);
        MEVENT_TRIGGER_NRET(evt, NULL, REQ_CMD_CITY_BY_S, FLAGS_SYNC);
        cityid = hdf_get_int_value(evt->hdfrcv, "city.id", 0);
    }

    evt = hash_lookup(evth, "member");
    if (!evt) return nerr_raise(NERR_ASSERT, "member null");

    hdf_copy(evt->hdfsnd, NULL, member);

    hdf_set_int_value(evt->hdfsnd, "cityid", cityid);
    hdf_set_int_value(evt->hdfsnd, "statu", MEMBER_ST_SPD_FRESH);
    if (hdf_get_int_value(plan, "dad", PLAN_DAD_MAN) == PLAN_DAD_CAR)
        hdf_set_value(evt->hdfsnd, "_addcar", "1");
    
    MEVENT_TRIGGER_NRET(evt, NULL, REQ_CMD_MEMBER_ADD, FLAGS_NONE);

    /*
     * plan
     */
    evt = hash_lookup(evth, "plan");
    if (!evt) return nerr_raise(NERR_ASSERT, "plan backend error");
    
    hdf_copy(evt->hdfsnd, NULL, plan);
    hdf_set_int_value(evt->hdfsnd, "cityid", cityid);
    hdf_set_int_value(evt->hdfsnd, "statu", PLAN_ST_SPD_FRESH);

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

    if (!cgi || !cgi->hdf || !db) return nerr_raise(NERR_ASSERT, "paramter null");

    MDB_PAGEDIV_SET(cgi->hdf, PRE_OUTPUT, db, "plan", "statu=%d",
                    NULL, PLAN_ST_SPD_RBTED);
    
    return STATUS_OK;
}

NEOERR* spd_post_do_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mdb_conn *db = hash_lookup(dbh, "plan");
	NEOERR *err;

    if (!cgi || !cgi->hdf || !db) return nerr_raise(NERR_ASSERT, "paramter null");

    int id = hdf_get_int_value(cgi->hdf, PRE_QUERY".exceptid", 0);

    MDB_QUERY_RAW(db, "plan", _COL_PLAN, "statu=%d AND id !=%d LIMIT 1",
                  NULL, PLAN_ST_SPD_RBTED, id);
    err = mdb_set_row(cgi->hdf, db, _COL_PLAN, PRE_OUTPUT".plan");
	if (err != STATUS_OK) return nerr_pass(err);
    
    db = hash_lookup(dbh, "member");
    if (!db) return nerr_raise(NERR_ASSERT, "member null");

    int mid = hdf_get_int_value(cgi->hdf, PRE_OUTPUT".plan.mid", 0);

    MDB_QUERY_RAW(db, "member", _COL_MEMBER_ADMIN, "mid=%d", NULL, mid);
    err = mdb_set_row(cgi->hdf, db, _COL_MEMBER_ADMIN, PRE_OUTPUT".member");
    if (err != STATUS_OK) return nerr_pass(err);

    int cityid = hdf_get_int_value(cgi->hdf, PRE_OUTPUT".plan.cityid", 0);

    MDB_QUERY_RAW(db, "city", _COL_CITY, "id=%d", NULL, cityid);
    mdb_set_row(cgi->hdf, db, _COL_CITY, PRE_OUTPUT".city");
    nerr_ignore(&err);
    
    return STATUS_OK;
}

NEOERR* spd_post_do_data_mod(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt;

    if (!cgi || !cgi->hdf) return nerr_raise(NERR_ASSERT, "paramter null");

    evt = hash_lookup(evth, "plan");
    if (!evt) return nerr_raise(NERR_ASSERT, "plan null");
    
    if (!hdf_get_obj(cgi->hdf, PRE_QUERY".plan"))
        return nerr_raise(NERR_ASSERT, "plan null");
    
    hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY".plan"));
    hdf_set_int_value(evt->hdfsnd, "statu", PLAN_ST_SPD_OK);

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_PLAN_UP, FLAGS_NONE);

    if (hdf_get_obj(cgi->hdf, PRE_QUERY".member")) {
        evt = hash_lookup(evth, "member");
        if (!evt) return nerr_raise(NERR_ASSERT, "member null");

        hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY".member"));
        MEVENT_TRIGGER(evt, NULL, REQ_CMD_MEMBER_UP, FLAGS_NONE);
    }

    return STATUS_OK;
}

NEOERR* spd_post_do_data_del(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt;

    if (!cgi || !cgi->hdf) return nerr_raise(NERR_ASSERT, "paramter null");

    evt = hash_lookup(evth, "plan");
    if (!evt) return nerr_raise(NERR_ASSERT, "plan null");
    
    if (!hdf_get_obj(cgi->hdf, PRE_QUERY".plan"))
        return nerr_raise(NERR_ASSERT, "plan null");
    
    hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY".plan"));
    hdf_set_int_value(evt->hdfsnd, "statu", PLAN_ST_DELETE);

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_PLAN_UP, FLAGS_NONE);

    return STATUS_OK;
}

NEOERR* spd_robot_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    return STATUS_OK;
}

NEOERR* spd_post_robot_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mdb_conn *db = hash_lookup(dbh, "plan");
    NEOERR *err;

    if (!cgi || !cgi->hdf || !db) return nerr_raise(NERR_ASSERT, "paramter null");
    
    MDB_EXEC(db, NULL, "SELECT " _COL_PLAN_C " FROM plan p, city c WHERE "
             "p.statu=%d AND c.id=p.cityid LIMIT 100", NULL, PLAN_ST_SPD_FRESH);
    err = mdb_set_rows(cgi->hdf, db, _COL_PLAN_C, PRE_OUTPUT".plans", NULL);
	if (err != STATUS_OK) return nerr_pass(err);

    hdf_set_attr(cgi->hdf, PRE_OUTPUT".plans", "type", "array");

    return STATUS_OK;
}

NEOERR* spd_post_robot_data_mod(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "plan");
    HDF *plan;
    
    if (!cgi || !cgi->hdf || !evt) return nerr_raise(NERR_ASSERT, "paramter null");

    HDF_GET_OBJ(cgi->hdf, PRE_QUERY".plan", plan);

    hdf_copy(evt->hdfsnd, NULL, plan);
    if (hdf_get_int_value(plan, "statu", PLAN_ST_SPD_FRESH) != PLAN_ST_SPD_RBTED)
        hdf_set_int_value(evt->hdfsnd, "statu", PLAN_ST_SPD_RBT_OK);
    
    MEVENT_TRIGGER(evt, NULL, REQ_CMD_PLAN_UP, FLAGS_NONE);

    return STATUS_OK;
}
