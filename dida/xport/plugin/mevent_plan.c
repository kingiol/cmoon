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

static NEOERR* plan_cmd_plan_add(struct plan_entry *e, QueueEntry *q)
{
	STRING str; string_init(&str);
    char *mname;
	NEOERR *err;

    mdb_conn *db = e->db;

    REQ_GET_PARAM_STR(q->hdfrcv, "mname", mname);

    hdf_set_int_value(q->hdfrcv, "mid", hash_string_rev(mname));

    err = mcs_build_incol(q->hdfrcv,
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

    err = mcs_build_upcol(q->hdfrcv,
                          hdf_get_obj(g_cfg, CONFIG_PATH".UpdateCol.appinfo"), &str);
	if (err != STATUS_OK) return nerr_pass(err);

    MDB_EXEC(db, NULL, "UPDATE plan SET %s WHERE id=%d;", NULL, str.buf, id);

    string_clear(&str);

    cache_delf(cd, PREFIX_PLAN"%d", id);
    
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
        break;
    case REQ_CMD_PLAN_ADD:
        err = plan_cmd_plan_add(e, q);
        break;
    case REQ_CMD_PLAN_UP:
        err = plan_cmd_plan_up(e, q);
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
