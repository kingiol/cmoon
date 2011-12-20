#include "mheads.h"
#include "lheads.h"
#include "oplan.h"

NEOERR* plan_match_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "plan");
    
    if (!cgi || !cgi->hdf || !evt) return nerr_raise(NERR_ASSERT, "paramter null");

    hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_PLAN_GET_BY_GEO, FLAGS_SYNC);

    hdf_copy(cgi->hdf, PRE_OUTPUT, evt->hdfrcv);
    hdf_set_attr(cgi->hdf, PRE_OUTPUT".plans", "type", "array");

    /*
     * fuck the time
     */

    return STATUS_OK;
}

NEOERR* plan_leave_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt;
    char *mname;
    HDF *plan;
    int id;
    
    if (!cgi || !cgi->hdf) return nerr_raise(NERR_ASSERT, "paramter null");

    evt = hash_lookup(evth, "member");
    if (!evt) return nerr_raise(NERR_ASSERT, "member null");

    HDF_GET_OBJ(cgi->hdf, PRE_QUERY".plan", plan);
    HDF_GET_STR(plan, "phone", mname);
    HDF_GET_INT(plan, "scityid", id);
    
    hdf_set_valuef(evt->hdfsnd, "mname=%s@%s", mname, SITE_DOMAIN);
    hdf_set_value(evt->hdfsnd, "phone", mname);
    hdf_set_int_value(evt->hdfsnd, "cityid", id);

    MEVENT_TRIGGER_NRET(evt, NULL, REQ_CMD_MEMBER_ADD, FLAGS_NONE);

    evt = hash_lookup(evth, "plan");
    if (!evt) return nerr_raise(NERR_ASSERT, "plan null");

    hdf_copy(evt->hdfsnd, NULL, plan);
    hdf_set_valuef(evt->hdfsnd, "mname=%s@%s", mname, SITE_DOMAIN);

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_PLAN_ADD, FLAGS_NONE);
    
    return STATUS_OK;
}
