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
