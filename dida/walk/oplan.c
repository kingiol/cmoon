#include "mheads.h"
#include "lheads.h"
#include "oplan.h"

static time_t m_thatsec = 0;

static int plan_compare(const void *a, const void *b)
{
    HDF *ha = *(HDF**)a, *hb = *(HDF**)b;
    unsigned int seca = mcs_get_uint_value(ha, "epochsec", 0);
    unsigned int secb = mcs_get_uint_value(hb, "epochsec", 0);

    return abs(seca - m_thatsec) - abs(secb - m_thatsec);
}

static void plan_prepare_time(HDF *node, char *date, struct tm *todaystm)
{
    if (!node || !date || !todaystm) return;

    char datetime[LEN_TM] = {0}, *stime, *sdate;
    time_t tm;
    stime = hdf_get_value(node, "stime", "08:00:00");
    sdate = hdf_get_value(node, "sdate", "2011-11-11");
    
    int repeat = hdf_get_int_value(node, "repeat", PLAN_RPT_NONE);
    if (repeat == PLAN_RPT_DAY) {
        /*
         * use date as datepart
         */
        snprintf(datetime, LEN_TM, "%s %s", date, stime);
    } else if (repeat == PLAN_RPT_WEEK) {
        /*
         * use the nearest date as datepart
         */
        int minday = 7, thatday;
        /* 1,2,3,4,5 */
        if (sdate) {
            int today = todaystm->tm_wday + 1;
            ULIST *list;
            
            string_array_split(&list, sdate, ",", 100);
            ITERATE_MLIST(list) {
                thatday = atoi(list->items[t_rsv_i]);
                if (abs(minday) > abs(thatday - today))
                    minday = thatday - today;
            }
            uListDestroy(&list, ULIST_FREE);
        }
        tm = m_thatsec + (minday*60*60*24);
        struct tm *stm = localtime(&tm);
        char s[LEN_DT];
        strftime(s, LEN_DT, "%Y-%m-%d", stm);
        snprintf(datetime, LEN_TM, "%s %s", s, stime);
    } else {
        /*
         * use sdate as datepart
         */
        snprintf(datetime, LEN_TM, "%s %s", sdate, stime);
    }

    struct tm thatdaystm;
    strptime(datetime, "%Y-%m-%d %H:%M:%S", &thatdaystm);
    tm = mktime(&thatdaystm);

    hdf_set_value(node, "datetime", datetime);
    mcs_set_uint_value(node, "epochsec", tm);
}

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
    char *pdate, *ptime;
    time_t todaytm;
    struct tm thatdaystm, *todaystm;
    char datetime[LEN_TM];
    
    HDF_GET_STR(cgi->hdf, PRE_QUERY".date", pdate);
    HDF_GET_STR(cgi->hdf, PRE_QUERY".time", ptime);

    snprintf(datetime, sizeof(datetime), "%s %s", pdate, ptime);
    strptime(datetime, "%Y-%m-%d %H:%M:%S", &thatdaystm);
    m_thatsec = mktime(&thatdaystm);
    
    todaytm = time(NULL);
    todaystm = localtime(&todaytm);
    
    HDF *node = hdf_get_child(cgi->hdf, PRE_OUTPUT".plans");
    while (node) {
        plan_prepare_time(node, pdate, todaystm);

        node = hdf_obj_next(node);
    }

    node = hdf_get_obj(cgi->hdf, PRE_OUTPUT".plans");
    if (node) hdf_sort_obj(node, plan_compare);

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
