#include "mevent_plugin.h"
#include "mevent_member.h"

#define PLUGIN_NAME    "member"
#define CONFIG_PATH    PRE_PLUGIN"."PLUGIN_NAME

struct member_stats {
    unsigned long msg_total;
    unsigned long msg_unrec;
    unsigned long msg_badparam;
    unsigned long msg_stats;

    unsigned long proc_suc;
    unsigned long proc_fai;
};

struct member_entry {
    EventEntry base;
    mdb_conn *db;
    Cache *cd;
    struct member_stats st;
};

static NEOERR* member_cmd_car_get(struct member_entry *e, QueueEntry *q)
{
	unsigned char *val = NULL; size_t vsize = 0;
    char *mname;
    int mid;
	NEOERR *err;

    mdb_conn *db = e->db;
    struct cache *cd = e->cd;

    if (!hdf_get_value(q->hdfrcv, "mid", NULL)) {
        REQ_GET_PARAM_STR(q->hdfrcv, "mname", mname);
        mid = hash_string_rev(mname);
    } else mid = hdf_get_int_value(q->hdfrcv, "mid", 0);

    if (cache_getf(cd, &val, &vsize, PREFIX_CAR"%d", mid)) {
        unpack_hdf(val, vsize, &q->hdfsnd);
    } else {
        MDB_QUERY_RAW(db, "car", _COL_CAR, "mid=%d", NULL, mid);
        err = mdb_set_row(q->hdfsnd, db, _COL_CAR, NULL);
        if (err != STATUS_OK) return nerr_pass(err);

        CACHE_HDF(q->hdfsnd, CAR_CC_SEC, PREFIX_CAR"%d", mid);
    }
    
    return STATUS_OK;
}

static NEOERR* member_cmd_mem_get(struct member_entry *e, QueueEntry *q)
{
	unsigned char *val = NULL; size_t vsize = 0;
    char *mname;
    int mid;
	NEOERR *err;

    mdb_conn *db = e->db;
    struct cache *cd = e->cd;

    if (hdf_get_value(q->hdfrcv, "mid", NULL)) {
        mid = hdf_get_int_value(q->hdfrcv, "mid", 0);
    } else {
        REQ_GET_PARAM_STR(q->hdfrcv, "mname", mname);
        mid = hash_string_rev(mname);
    }
    
    if (cache_getf(cd, &val, &vsize, PREFIX_MEMBER"%d", mid)) {
        unpack_hdf(val, vsize, &q->hdfsnd);
    } else {
        MDB_QUERY_RAW(db, "member", _COL_MEMBER, "mid=%d", NULL, mid);
        err = mdb_set_row(q->hdfsnd, db, _COL_MEMBER, NULL);
        if (err != STATUS_OK) return nerr_pass(err);

        MDB_QUERY_RAW(db, "car", _COL_CAR, "mid=%d", NULL, mid);
        err = mdb_set_row(q->hdfsnd, db, _COL_CAR, NULL);
        nerr_handle(&err, NERR_NOT_FOUND);
        if (err != STATUS_OK) return nerr_pass(err);

        CACHE_HDF(q->hdfsnd, MEMBER_CC_SEC, PREFIX_MEMBER"%d", mid);
    }
    
    return STATUS_OK;
}

static NEOERR* member_cmd_mem_priv_get(struct member_entry *e, QueueEntry *q)
{
	unsigned char *val = NULL; size_t vsize = 0;
    char *mname;
    int mid;
	NEOERR *err;

    mdb_conn *db = e->db;
    struct cache *cd = e->cd;

    if (hdf_get_value(q->hdfrcv, "mid", NULL)) {
        mid = hdf_get_int_value(q->hdfrcv, "mid", 0);
    } else {
        REQ_GET_PARAM_STR(q->hdfrcv, "mname", mname);
        mid = hash_string_rev(mname);
    }
    
    if (cache_getf(cd, &val, &vsize, PREFIX_MEMBER_PRIV"%d", mid)) {
        unpack_hdf(val, vsize, &q->hdfsnd);
    } else {
        MDB_QUERY_RAW(db, "member", _COL_MEMBER_PRIV, "mid=%d", NULL, mid);
        err = mdb_set_row(q->hdfsnd, db, _COL_MEMBER_PRIV, NULL);
        if (err != STATUS_OK) return nerr_pass(err);

        CACHE_HDF(q->hdfsnd, MEMBER_CC_SEC, PREFIX_MEMBER_PRIV"%d", mid);
    }
    
    return STATUS_OK;
}

static NEOERR* member_cmd_car_add(struct member_entry *e, QueueEntry *q)
{
	STRING str; string_init(&str);
    char *mname;
    int mid;
	NEOERR *err;

    mdb_conn *db = e->db;

    if (!hdf_get_value(q->hdfrcv, "mid", NULL)) {
        REQ_GET_PARAM_STR(q->hdfrcv, "mname", mname);
        hdf_set_int_value(q->hdfrcv, "mid", hash_string_rev(mname));
    }
    mid = hdf_get_int_value(q->hdfrcv, "mid", 0);

    err = member_cmd_car_get(e, q);
    nerr_handle(&err, NERR_NOT_FOUND);
    if (hdf_get_obj(q->hdfsnd, "size"))
        return nerr_raise(REP_ERR_CARED, "%d already has car", mid);
    
    err = mdb_build_incol(q->hdfrcv,
                          hdf_get_obj(g_cfg, CONFIG_PATH".InsertCol.car"),
                          &str);
	if (err != STATUS_OK) return nerr_pass(err);
    
    MDB_EXEC(db, NULL, "INSERT INTO car %s", NULL, str.buf);
    
    string_clear(&str);

    return STATUS_OK;
}

static NEOERR* member_cmd_mem_add(struct member_entry *e, QueueEntry *q)
{
	STRING str; string_init(&str);
    char *mname, *ori = NULL, *tmps;
	NEOERR *err;

    mdb_conn *db = e->db;

    REQ_GET_PARAM_STR(q->hdfrcv, "mname", mname);
    REQ_FETCH_PARAM_STR(q->hdfrcv, "ori", ori);

    if (ori) {
        tmps = hdf_get_valuef(g_cfg, "Odomain.%s", ori);
        if (tmps) hdf_set_value(q->hdfrcv, "ori", tmps);
    }

    hdf_set_int_value(q->hdfrcv, "mid", hash_string_rev(mname));

    err = member_cmd_mem_get(e, q);
    nerr_handle(&err, NERR_NOT_FOUND);
	if (err != STATUS_OK) return nerr_pass(err);

    if (hdf_get_obj(q->hdfsnd, "male"))
        return nerr_raise(REP_ERR_MEMBERED, "%s already regist", mname);
    
    err = mdb_build_incol(q->hdfrcv,
                          hdf_get_obj(g_cfg, CONFIG_PATH".InsertCol.member"),
                          &str);
	if (err != STATUS_OK) return nerr_pass(err);
    
    MDB_EXEC(db, NULL, "INSERT INTO member %s", NULL, str.buf);
    
    string_clear(&str);
    
    tmps = NULL;
    REQ_FETCH_PARAM_STR(q->hdfrcv, "_addcar", tmps);
    if (tmps) return nerr_pass(member_cmd_car_add(e, q));

    return STATUS_OK;
}

static NEOERR* member_cmd_mem_up(struct member_entry *e, QueueEntry *q)
{
	STRING str; string_init(&str);
    int mid;
	NEOERR *err;

    mdb_conn *db = e->db;
    struct cache *cd = e->cd;

    REQ_GET_PARAM_INT(q->hdfrcv, "mid", mid);

    err = member_cmd_mem_get(e, q);
	if (err != STATUS_OK) return nerr_pass(err);

    if (!hdf_get_obj(q->hdfsnd, "mid"))
        return nerr_raise(REP_ERR_MEMBER_NEXIST, "member %d not exist", mid);

    err = mdb_build_upcol(q->hdfrcv,
                          hdf_get_obj(g_cfg, CONFIG_PATH".UpdateCol.member"), &str);
	if (err != STATUS_OK) return nerr_pass(err);

    MDB_EXEC(db, NULL, "UPDATE member SET %s WHERE mid=%d;", NULL, str.buf, mid);

    string_clear(&str);

    cache_delf(cd, PREFIX_MEMBER"%d", mid);
    cache_delf(cd, PREFIX_MEMBER_PRIV"%d", mid);
    
    return STATUS_OK;
}

static void member_process_driver(EventEntry *entry, QueueEntry *q)
{
    struct member_entry *e = (struct member_entry*)entry;
    NEOERR *err;
    int ret;
    
    struct member_stats *st = &(e->st);

    st->msg_total++;
    
    mtc_dbg("process cmd %u", q->operation);
    switch (q->operation) {
        CASE_SYS_CMD(q->operation, q, e->cd, err);
    case REQ_CMD_MEMBER_GET:
        err = member_cmd_mem_get(e, q);
        break;
    case REQ_CMD_MEMBER_PRIV_GET:
        err = member_cmd_mem_priv_get(e, q);
        break;
    case REQ_CMD_MEMBER_ADD:
        err = member_cmd_mem_add(e, q);
        break;
    case REQ_CMD_MEMBER_UP:
        err = member_cmd_mem_up(e, q);
        break;
    case REQ_CMD_CAR_GET:
        err = member_cmd_car_get(e, q);
        break;
    case REQ_CMD_CAR_ADD:
        err = member_cmd_car_add(e, q);
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

static void member_stop_driver(EventEntry *entry)
{
    struct member_entry *e = (struct member_entry*)entry;

    /*
     * e->base.name, e->base will free by mevent_stop_driver() 
     */
    mdb_destroy(e->db);
    cache_free(e->cd);
}



static EventEntry* member_init_driver(void)
{
    struct member_entry *e = calloc(1, sizeof(struct member_entry));
    if (e == NULL) return NULL;
    NEOERR *err;

    e->base.name = (unsigned char*)strdup(PLUGIN_NAME);
    e->base.ksize = strlen(PLUGIN_NAME);
    e->base.process_driver = member_process_driver;
    e->base.stop_driver = member_stop_driver;
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

struct event_driver member_driver = {
    .name = (unsigned char*)PLUGIN_NAME,
    .init_driver = member_init_driver,
};
