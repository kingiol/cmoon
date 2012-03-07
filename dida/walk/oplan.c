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

    HDF_GET_OBJ(cgi->hdf, PRE_QUERY".plan", plan);
    
    evt = hash_lookup(evth, "plan");
    if (!evt) return nerr_raise(NERR_ASSERT, "plan null");

    hdf_copy(evt->hdfsnd, NULL, plan);
    hdf_set_value(evt->hdfsnd, "mid", "0");

    /*
     * add plan
     */
    MEVENT_TRIGGER(evt, NULL, REQ_CMD_PLAN_ADD, FLAGS_SYNC);
    id = hdf_get_int_value(evt->hdfrcv, "id", 0);
    if (id <= 0) return nerr_raise(NERR_ASSERT, "添加路线失败");
    hdf_set_int_value(plan, "id", id);

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
    hdf_set_int_value(evt->hdfsnd, "id", id);

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_FFT_EXPECT_MATCH, FLAGS_SYNC);

    /*
     * these man need notify them immediately
     */
    HDF *obj = hdf_get_obj(evt->hdfrcv, "expects");
    if (obj) {
        /*
         * inbox notify
         */
        hdf_copy(obj, PRE_DATASET".Output.plans.0", plan);

        err = inbox_multi_add(obj, evth, "PlanMatched");
        if (err != STATUS_OK) return nerr_pass(err);

        /*
         * email notify
         */
        HDF *child, *tnode;
        hdf_init(&tnode);
        child = hdf_obj_child(obj);
        while (child) {
            if (hdf_get_int_value(child, "addrtype", FFT_EXPECT_NONE) ==
                FFT_EXPECT_EMAIL) {
                hdf_copy(tnode, hdf_obj_name(child), child);
            }
            
            child = hdf_obj_next(child);
        }
        hdf_copy(tnode, PRE_DATASET".Output.plans.0", plan);

        err = email_multi_add(tnode, evth, "PlanMatched");
        if (err != STATUS_OK) return nerr_pass(err);

        hdf_destroy(&tnode);

        /*
         * TODO sms notify
         */
    }
    
    return STATUS_OK;
}

NEOERR* plan_info_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "plan");

    MCS_NOT_NULLB(cgi, evt);

    hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_PLAN_GET, FLAGS_SYNC);

    hdf_copy(cgi->hdf, PRE_OUTPUT".plan", evt->hdfrcv);
    
    return STATUS_OK;
}

NEOERR* plan_pic_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "plan");
    HDF *node;
    char *s = NULL, *defs = NULL;
    NEOERR *err;
    
    MCS_NOT_NULLB(cgi->hdf, evt);

    HDF_FETCH_STR(cgi->hdf, PRE_QUERY".defs", defs);

    node = hdf_get_child(cgi->hdf, PRE_QUERY".type");
    if (!node) HDF_GET_STR(cgi->hdf, PRE_QUERY".type", s);
    
    hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_PLAN_PRIV_GET, FLAGS_SYNC);

    if (s) goto getval;
    while (node) {
        s = hdf_obj_value(node);
    getval:
        /*
         * turn &amp; into & in url format
         */
        mstr_html_unescape(evt->hdfrcv, s);
        s = hdf_get_value(evt->hdfrcv, s, NULL);
        if (s && *s) {
            if (!strncmp(s, "http:", 5)) {
                hdf_set_value(cgi->hdf, PRE_OUTPUT".302", s);
                return STATUS_OK;
            }
            break;
        }
        
        node = hdf_obj_next(node);
    }

    if (!s || !*s) {
        if (!defs) s = SITE_DOMAIN;
        else if (!strcmp(defs, "segv")) return STATUS_OK;
        else s = defs;
    }
    
    err = mimg_create_from_string(s,
                                  hdf_get_value(g_cfg,
                                                "Config.font.plan.path",
                                                "/usr/share/ttf/Times.ttf"),
                                  atof(hdf_get_value(g_cfg,
                                                     "Config.font.plan.size",
                                                     "14.")),
                                  &ses->data);
    if (err != STATUS_OK) return nerr_pass(err);
    
    return STATUS_OK;
}
