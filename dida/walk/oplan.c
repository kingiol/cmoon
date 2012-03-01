#include "mheads.h"
#include "lheads.h"
#include "oplan.h"

NEOERR* plan_match_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "plan");
    
    if (!cgi || !cgi->hdf || !evt) return nerr_raise(NERR_ASSERT, "paramter null");

    hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_PLAN_MATCH, FLAGS_SYNC);

    hdf_copy(cgi->hdf, PRE_OUTPUT, evt->hdfrcv);
    hdf_set_attr(cgi->hdf, PRE_OUTPUT".plans", "type", "array");

    return STATUS_OK;
}

NEOERR* plan_leave_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt;
    char *mname;
    HDF *plan;
    int id, expect;
    NEOERR *err;
    
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

    /*
     * add plan
     */
    MEVENT_TRIGGER(evt, NULL, REQ_CMD_PLAN_ADD, FLAGS_SYNC);
    id = hdf_get_int_value(evt->hdfrcv, "id", 0);
    if (id <= 0) return nerr_raise(NERR_ASSERT, "添加路线失败");

    /*
     * add subscribe
     */
    expect = hdf_get_int_value(plan, "subscribe", FFT_EXPECT_NONE);
    if (expect != FFT_EXPECT_NONE) {
        MEMBER_CHECK_LOGIN();

        evt = hash_lookup(evth, "fft");
        MCS_NOT_NULLA(evt);

        hdf_copy(evt->hdfsnd, NULL, plan);
            
        hdf_set_int_value(evt->hdfsnd, "addrtype", expect);
        hdf_set_value(evt->hdfsnd, "mname", mname);
        hdf_set_int_value(evt->hdfsnd, "pid", id);

        MEVENT_TRIGGER(evt, NULL, REQ_CMD_FFT_EXPECT_ADD, FLAGS_NONE);
    }

    /*
     * meet expect
     */
    evt = hash_lookup(evth, "fft");
    MCS_NOT_NULLA(evt);

    hdf_copy(evt->hdfsnd, NULL, plan);

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_FFT_EXPECT_MATCH, FLAGS_SYNC);

    /*
     * these man need notify them immediately
     */
    HDF *obj = hdf_get_obj(evt->hdfrcv, "expects");
    if (obj) {
        hdf_set_valuef(obj, PRE_DATASET".message.plans=<a target=\"_blank\" "   \
                       "href=\"/plan/%d\">%d</a>", id, id);
        err = inbox_multi_add(obj, evth, "PlanMatched");
        if (err != STATUS_OK) return nerr_pass(err);

        hdf_remove_tree(obj, PRE_DATASET);
        hdf_set_valuef(obj, PRE_DATASET".content.plans=<a target=\"_blank\" "   \
                       "href=\"http://%s/plan/%d\">%d</a>", SITE_DOMAIN, id, id);
        err = email_multi_add(obj, evth, "PlanMatched");
        if (err != STATUS_OK) return nerr_pass(err);
    }
    
    return STATUS_OK;
}
