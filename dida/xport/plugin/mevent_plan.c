#include "mevent_plugin.h"
#include "mevent_plan.h"

#define PLUGIN_NAME    "plan"
#define CONFIG_PATH    PRE_PLUGIN"."PLUGIN_NAME

struct plan_stats {
    unsigned long msg_total;
    unsigned long msg_unrec;
    unsigned long msg_badparam;
    unsigned long msg_stats;

    unsigned long proc_suc;
    unsigned long proc_fai;
};

struct plan_entry {
    EventEntry base;
    mdb_conn *db;
    Cache *cd;
    struct plan_stats st;
};

static bool plan_spd_exist(struct plan_entry *e, QueueEntry *q,
                           char *ori, char *id)
{
	unsigned char *val = NULL; size_t vsize = 0;
	NEOERR *err;

    if (!ori || !id) return false;
    
    mdb_conn *db = e->db;
    struct cache *cd = e->cd;
    
    ori = hdf_get_valuef(g_cfg, "Odomain.%s", ori);
    
    if (!cache_getf(cd, &val, &vsize, PREFIX_SPD"%s%s", ori, id)) {
        MDB_QUERY_RAW(db, "plan", "mid", "ori=$1 AND oid=$2", "ss", ori, id);
        err = mdb_set_row(q->hdfsnd, db, "mid", NULL);
        if(nerr_handle(&err, NERR_NOT_FOUND)) return false;
        nerr_ignore(&err);
        
        CACHE_HDF(q->hdfsnd, SPD_CC_SEC, PREFIX_SPD"%s%s", ori, id);
    }
    return true;
}

static NEOERR* plan_cmd_plan_get(struct plan_entry *e, QueueEntry *q)
{
	unsigned char *val = NULL; size_t vsize = 0;
    int id;
	NEOERR *err;

    mdb_conn *db = e->db;
    struct cache *cd = e->cd;
    
    REQ_GET_PARAM_INT(q->hdfrcv, "id", id);

    if (cache_getf(cd, &val, &vsize, PREFIX_PLAN"%d", id)) {
        unpack_hdf(val, vsize, &q->hdfsnd);
    } else {
        MDB_QUERY_RAW(db, "plan", _COL_PLAN, "id=%d", NULL, id);
        err = mdb_set_row(q->hdfsnd, db, _COL_PLAN, NULL);
        if (err != STATUS_OK) return nerr_pass(err);

        CACHE_HDF(q->hdfsnd, PLAN_CC_SEC, PREFIX_PLAN"%d", id);
    }
    
    return STATUS_OK;
}

/*
 * get plans by geographically (must be very fast...)
 * keep plan clean (no history useless plan) please
 * limit by:
 *    dad, statu, (scityid, ecityid) / rect
 */
static NEOERR* plan_cmd_plan_get_by_geo(struct plan_entry *e, QueueEntry *q)
{
	STRING str; string_init(&str);
	unsigned char *val = NULL; size_t vsize = 0;
	NEOERR *err;

    mdb_conn *db = e->db;
    struct cache *cd = e->cd;
    
    int dad, scityid = 0, ecityid = 0, ttnum;
    char *rect = NULL;

    REQ_GET_PARAM_INT(q->hdfrcv, "dad", dad);
    REQ_FETCH_PARAM_INT(q->hdfrcv, "scityid", scityid);
    REQ_FETCH_PARAM_INT(q->hdfrcv, "ecityid", ecityid);
    REQ_FETCH_PARAM_STR(q->hdfrcv, "rect", rect);
    
    hdf_set_int_value(q->hdfrcv, "statu", PLAN_ST_PAUSE);

    if ((scityid == 0 || ecityid == 0) && rect == NULL)
        return nerr_raise(REP_ERR_BADPARAM, "paramter null");

    if (cache_getf(cd, &val, &vsize, PREFIX_PLAN"%d_%d_%d_%s",
                   dad, scityid, ecityid, rect)) {
        unpack_hdf(val, vsize, &q->hdfsnd);
    } else {
        err = mdb_build_querycond(q->hdfrcv,
                                  hdf_get_obj(g_cfg, CONFIG_PATH".QueryCond.geoa"),
                                  &str, NULL);
        if (err != STATUS_OK) return nerr_pass(err);

        MDB_QUERY_RAW(db, "plan", _COL_PLAN, "%s", NULL, str.buf);
        err = mdb_set_rows(q->hdfsnd, db, _COL_PLAN, "plans", "0");
        ttnum = mdb_get_rows(db);

        if (nerr_handle(&err, NERR_NOT_FOUND) ||
            (ttnum < hdf_get_int_value(g_cfg, CONFIG_PATH".geoaMin", 0)) ) {
            /*
             * too few plans
             */
        geob:
            string_clear(&str);
            mdb_build_querycond(q->hdfrcv,
                                hdf_get_obj(g_cfg, CONFIG_PATH".QueryCond.geob"),
                                &str, NULL);
            MDB_QUERY_RAW(db, "plan", _COL_PLAN, "%s", NULL, str.buf);
            err = mdb_set_rows(q->hdfsnd, db, _COL_PLAN, "plans", "0");
            ttnum = mdb_get_rows(db);

            if (nerr_handle(&err, NERR_NOT_FOUND)) {
                /*
                 * ok, try larger at last
                 */
                string_clear(&str);
                mdb_build_querycond(q->hdfrcv,
                                    hdf_get_obj(g_cfg, CONFIG_PATH".QueryCond.geox"),
                                    &str, NULL);
                MDB_QUERY_RAW(db, "plan", _COL_PLAN, "%s", NULL, str.buf);
                err = mdb_set_rows(q->hdfsnd, db, _COL_PLAN, "plans", "0");

                ttnum = mdb_get_rows(db);
                if (nerr_handle(&err, NERR_NOT_FOUND))
                    return nerr_raise(REP_ERR_PLAN_NMATCH, "no result %d %d %s",
                                      scityid, ecityid, rect);
            }
            if (err != STATUS_OK) return nerr_pass(err);
        } else if (ttnum > hdf_get_int_value(g_cfg, CONFIG_PATH".geoaMax", 0)) {
            /*
             * too many plans
             */
            hdf_destroy(&q->hdfsnd);
            hdf_init(&q->hdfsnd);
            goto geob;
        }

        mtc_foo("get %d results", ttnum);
        hdf_set_int_value(q->hdfsnd, "_ntt", ttnum);
        CACHE_HDF(q->hdfsnd, PLAN_CC_SEC, PREFIX_PLAN"%d_%d_%d_%s",
                  dad, scityid, ecityid, rect);
    }
    
    string_clear(&str);
    
    return STATUS_OK;
}

static NEOERR* plan_cmd_plan_add(struct plan_entry *e, QueueEntry *q)
{
	STRING str; string_init(&str);
    char *mname, *ori = NULL, *oid = NULL, *tmps;
	NEOERR *err;

    mdb_conn *db = e->db;

    REQ_FETCH_PARAM_STR(q->hdfrcv, "ori", ori);
    REQ_FETCH_PARAM_STR(q->hdfrcv, "oid", oid);
    
    if (!hdf_get_value(q->hdfrcv, "mid", NULL)) {
        REQ_GET_PARAM_STR(q->hdfrcv, "mname", mname);
        hdf_set_int_value(q->hdfrcv, "mid", hash_string_rev(mname));
    }
    
    if (ori) {
        if (plan_spd_exist(e, q, ori, oid))
            return nerr_raise(REP_ERR_PLANED, "%s %s planed", ori, oid);
        
        tmps = hdf_get_valuef(g_cfg, "Odomain.%s", ori);
        if (tmps) hdf_set_value(q->hdfrcv, "ori", tmps);
    }


    err = mdb_build_incol(q->hdfrcv,
                          hdf_get_obj(g_cfg, CONFIG_PATH".InsertCol.plan"),
                          &str);
	if (err != STATUS_OK) return nerr_pass(err);

    MDB_EXEC(db, NULL, "INSERT INTO plan %s", NULL, str.buf);

    string_clear(&str);

    return STATUS_OK;
}

static NEOERR* plan_cmd_plan_up(struct plan_entry *e, QueueEntry *q)
{
	STRING str; string_init(&str);
    int id;
	NEOERR *err;

    mdb_conn *db = e->db;
    struct cache *cd = e->cd;

    REQ_GET_PARAM_INT(q->hdfrcv, "id", id);

    err = plan_cmd_plan_get(e, q);
	if (err != STATUS_OK) return nerr_pass(err);

    if (!hdf_get_obj(q->hdfsnd, "mid"))
        return nerr_raise(REP_ERR_PLAN_NEXIST, "plan %d not exist", id);

    err = mdb_build_upcol(q->hdfrcv,
                          hdf_get_obj(g_cfg, CONFIG_PATH".UpdateCol.plan"), &str);
	if (err != STATUS_OK) return nerr_pass(err);

    MDB_EXEC(db, NULL, "UPDATE plan SET %s WHERE id=%d;", NULL, str.buf, id);

    string_clear(&str);

    cache_delf(cd, PREFIX_PLAN"%d", id);
    
    return STATUS_OK;
}

static NEOERR* plan_cmd_spd_peel(struct plan_entry *e, QueueEntry *q)
{
    char *ori, *id;
    HDF *oids;
    int cnt = 0;

    REQ_GET_PARAM_STR(q->hdfrcv, "ori", ori);
    REQ_GET_PARAM_CHILD(q->hdfrcv, "oids", oids);

    while (oids) {
        id = hdf_obj_value(oids);
        if (!plan_spd_exist(e, q, ori, id)) {
            hdf_set_valuef(q->hdfsnd, "oids.%d=%s", cnt++, id);
        }
        oids = hdf_obj_next(oids);
    }

    return STATUS_OK;
}

static void plan_process_driver(EventEntry *entry, QueueEntry *q)
{
    struct plan_entry *e = (struct plan_entry*)entry;
    NEOERR *err;
    int ret;
    
    struct plan_stats *st = &(e->st);

    st->msg_total++;
    
    mtc_dbg("process cmd %u", q->operation);
    switch (q->operation) {
        CASE_SYS_CMD(q->operation, q, e->cd, err);
    case REQ_CMD_PLAN_GET:
        err = plan_cmd_plan_get(e, q);
        break;
    case REQ_CMD_PLAN_GET_BY_GEO:
        err = plan_cmd_plan_get_by_geo(e, q);
        break;
    case REQ_CMD_PLAN_ADD:
        err = plan_cmd_plan_add(e, q);
        break;
    case REQ_CMD_PLAN_UP:
        err = plan_cmd_plan_up(e, q);
        break;
    case REQ_CMD_SPD_PEEL:
        err = plan_cmd_spd_peel(e, q);
        break;
    case REQ_CMD_STATS:
        st->msg_stats++;
        err = STATUS_OK;
        hdf_set_int_value(q->hdfsnd, "msg_total", st->msg_total);
        hdf_set_int_value(q->hdfsnd, "msg_unrec", st->msg_unrec);
        hdf_set_int_value(q->hdfsnd, "msg_badparam", st->msg_badparam);
        hdf_set_int_value(q->hdfsnd, "msg_stats", st->msg_stats);
        hdf_set_int_value(q->hdfsnd, "proc_suc", st->proc_suc);
        hdf_set_int_value(q->hdfsnd, "proc_fai", st->proc_fai);
        break;
    default:
        st->msg_unrec++;
        err = nerr_raise(REP_ERR_UNKREQ, "unknown command %u", q->operation);
        break;
    }
    
    NEOERR *neede = mcs_err_valid(err);
    ret = neede ? neede->error : REP_OK;
    if (PROCESS_OK(ret)) {
        st->proc_suc++;
    } else {
        st->proc_fai++;
        if (ret == REP_ERR_BADPARAM) {
            st->msg_badparam++;
        }
        TRACE_ERR(q, ret, err);
    }
    if (q->req->flags & FLAGS_SYNC) {
            reply_trigger(q, ret);
    }
}

static void plan_stop_driver(EventEntry *entry)
{
    struct plan_entry *e = (struct plan_entry*)entry;

    /*
     * e->base.name, e->base will free by mevent_stop_driver() 
     */
    mdb_destroy(e->db);
    cache_free(e->cd);
}



static EventEntry* plan_init_driver(void)
{
    struct plan_entry *e = calloc(1, sizeof(struct plan_entry));
    if (e == NULL) return NULL;
    NEOERR *err;

    e->base.name = (unsigned char*)strdup(PLUGIN_NAME);
    e->base.ksize = strlen(PLUGIN_NAME);
    e->base.process_driver = plan_process_driver;
    e->base.stop_driver = plan_stop_driver;
    //mevent_add_timer(&e->base.timers, 60, true, hint_timer_up_term);

    char *s = hdf_get_value(g_cfg, CONFIG_PATH".dbsn", NULL);
    err = mdb_init(&e->db, s);
    JUMP_NOK(err, error);
    
    e->cd = cache_create(hdf_get_int_value(g_cfg, CONFIG_PATH".numobjs", 1024), 0);
    if (e->cd == NULL) {
        wlog("init cache failure");
        goto error;
    }
    
    return (EventEntry*)e;
    
error:
    if (e->base.name) free(e->base.name);
    if (e->db) mdb_destroy(e->db);
    if (e->cd) cache_free(e->cd);
    free(e);
    return NULL;
}

struct event_driver plan_driver = {
    .name = (unsigned char*)PLUGIN_NAME,
    .init_driver = plan_init_driver,
};
