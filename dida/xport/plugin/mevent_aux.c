#include "mevent_plugin.h"
#include "mevent_aux.h"

#define PLUGIN_NAME    "aux"
#define CONFIG_PATH    PRE_PLUGIN"."PLUGIN_NAME

struct aux_stats {
    unsigned long msg_total;
    unsigned long msg_unrec;
    unsigned long msg_badparam;
    unsigned long msg_stats;

    unsigned long proc_suc;
    unsigned long proc_fai;
};

struct aux_entry {
    EventEntry base;
    mdb_conn *db;
    Cache *cd;
    struct aux_stats st;
};

static NEOERR* aux_cmd_mail_add(struct aux_entry *e, QueueEntry *q)
{
    STRING str; string_init(&str);
    char sum[LEN_MD5], *content;
    NEOERR *err;

    REQ_GET_PARAM_STR(q->hdfrcv, "content", content);
    mstr_md5_str(content, sum);
    hdf_set_value(q->hdfrcv, "checksum", sum);
    
    mdb_conn *db = e->db;

    err = mdb_build_incol(q->hdfrcv,
                          hdf_get_obj(g_cfg, CONFIG_PATH".InsertCol.email"),
                          &str);
    if (err != STATUS_OK) return nerr_pass(err);
    
    MDB_EXEC(db, NULL, "INSERT INTO email %s", NULL, str.buf);
    string_clear(&str);

    return STATUS_OK;
}

static void aux_process_driver(EventEntry *entry, QueueEntry *q)
{
    struct aux_entry *e = (struct aux_entry*)entry;
    NEOERR *err;
    int ret;
    
    struct aux_stats *st = &(e->st);

    st->msg_total++;
    
    mtc_dbg("process cmd %u", q->operation);
    switch (q->operation) {
        CASE_SYS_CMD(q->operation, q, e->cd, err);
    case REQ_CMD_CMT_GET:
        //err = aux_cmd_cmt_get(e, q);
        break;
    case REQ_CMD_CMT_ADD:
        //err = aux_cmd_cmt_add(e, q);
        break;
    case REQ_CMD_MAIL_ADD:
        err = aux_cmd_mail_add(e, q);
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

static void aux_stop_driver(EventEntry *entry)
{
    struct aux_entry *e = (struct aux_entry*)entry;

    /*
     * e->base.name, e->base will free by mevent_stop_driver() 
     */
    mdb_destroy(e->db);
    cache_free(e->cd);
}



static EventEntry* aux_init_driver(void)
{
    struct aux_entry *e = calloc(1, sizeof(struct aux_entry));
    if (e == NULL) return NULL;
    NEOERR *err;

    e->base.name = (unsigned char*)strdup(PLUGIN_NAME);
    e->base.ksize = strlen(PLUGIN_NAME);
    e->base.process_driver = aux_process_driver;
    e->base.stop_driver = aux_stop_driver;
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

struct event_driver aux_driver = {
    .name = (unsigned char*)PLUGIN_NAME,
    .init_driver = aux_init_driver,
};
