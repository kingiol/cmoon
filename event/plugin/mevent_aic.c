#include "mevent_plugin.h"
#include "mevent_aic.h"

#define PLUGIN_NAME	"aic"
#define CONFIG_PATH	PRE_PLUGIN"."PLUGIN_NAME

struct aic_stats {
	unsigned long msg_total;
	unsigned long msg_unrec;
	unsigned long msg_badparam;
	unsigned long msg_stats;

	unsigned long proc_suc;
	unsigned long proc_fai;
};

struct aic_entry {
	struct event_entry base;
	mdb_conn *db;
	struct cache *cd;
	struct aic_stats st;
};

#define APPINFO_COL " aid, aname, pid, asn, masn, email, state, tune, "	\
	" to_char(intime, 'YYYY-MM-DD') as intime, "						\
	" to_char(uptime, 'YYYY-MM-DD') as uptime "

#define USERINFO_COL " uid, uname, ip, intime, aid, aname, "	\
	" to_char(intime, 'YYYY-MM-DD') as intime, "				\
	" to_char(uptime, 'YYYY-MM-DD') as uptime "

/*
 * input : aname(STR)
 * return: NORMAL
 * reply : ["state": 0, ...] OR []
 */
static int aic_cmd_appinfo(struct queue_entry *q, struct cache *cd, mdb_conn *db)
{
	unsigned char *val = NULL; size_t vsize = 0;
	int aid, pid;
	char *aname;

	REQ_GET_PARAM_STR(q->hdfrcv, "aname", aname);
	aid = hash_string(aname);

	if (cache_getf(cd, &val, &vsize, PREFIX_APPINFO"%d", aid)) {
		unpack_hdf(val, vsize, &q->hdfsnd);
	} else {
		hdf_set_value(q->hdfsnd, "pname", aname);
		MDB_QUERY_RAW(db, "appinfo", APPINFO_COL, "aid=%d", NULL, aid);
		if (mdb_set_row(q->hdfsnd, db, APPINFO_COL, NULL) == MDB_ERR_NONE) {
			pid = hdf_get_int_value(q->hdfsnd, "pid", 0);
			if (pid != 0) {
				MDB_QUERY_RAW(db, "appinfo", "aname", "aid=%d", NULL, pid);
				mdb_set_row(q->hdfsnd, db, "pname", NULL);
			}
			MDB_QUERY_RAW(db, "appinfo", "COUNT(*)+1 AS numuser", "pid=%d",
						  NULL, aid);
			mdb_set_row(q->hdfsnd, db, "numuser", NULL);
		}
		CACHE_HDF(q->hdfsnd, AIC_CC_SEC, PREFIX_APPINFO"%d", aid);
	}
	
	return REP_OK;
}

/*
 * input : aname(STR) asn(STR) masn(STR) email(STR) state(INT)
 * return: NORMAL REP_ERR_ALREADYREGIST
 * reply : NULL
 */
static int aic_cmd_appnew(struct queue_entry *q, struct cache *cd, mdb_conn *db)
{
	char *aname, *pname, *asn, *masn, *email;
	int aid, pid = 0, state, ret;

	REQ_GET_PARAM_INT(q->hdfrcv, "state", state);
	REQ_GET_PARAM_STR(q->hdfrcv, "aname", aname);
	REQ_GET_PARAM_STR(q->hdfrcv, "asn", asn);
	REQ_GET_PARAM_STR(q->hdfrcv, "masn", masn);
	REQ_GET_PARAM_STR(q->hdfrcv, "email", email);

	REQ_FETCH_PARAM_STR(q->hdfrcv, "pname", pname);

	aid = hash_string(aname);
	if (pname) pid = hash_string(pname);

	ret = aic_cmd_appinfo(q, cd, db);
	if (PROCESS_NOK(ret)) {
		mtc_err("info get failure %s", aname);
		return ret;
	}

	if (hdf_get_obj(q->hdfsnd, "state")) {
		mtc_warn("%s already regist", aname);
		return REP_ERR_ALREADYREGIST;
	}

	MDB_EXEC_EVT(db, NULL, "INSERT INTO appinfo (aid, aname, "
				 " pid, asn, masn, email, state) "
				 " VALUES ($1, $2::varchar(256), $3, $4::varchar(256), "
				 " $5::varchar(256), $6::varchar(256), $7);",
				 "isisssi", aid, aname, pid, asn, masn, email, state);
	
	cache_delf(cd, PREFIX_APPINFO"%d", aid);
	if (pid > 0) {
		cache_delf(cd, PREFIX_APPOUSER"%d_0", pid);
	}

	return REP_OK;
}

/*
 * input : aname(STR) [asn(STR) masn(STR) email(STR) state(INT)]
 * return: NORMAL REP_ERR_ALREADYREGIST
 * reply : NULL
 */
static int aic_cmd_appup(struct queue_entry *q, struct cache *cd, mdb_conn *db)
{
	char *aname;
	int aid, pid = 0, tune = -1, ret;
	STRING str;	string_init(&str);
	
	REQ_GET_PARAM_STR(q->hdfrcv, "aname", aname);
	REQ_FETCH_PARAM_INT(q->hdfrcv, "tune", tune);
	if (tune != -1) {
		if (hdf_get_int_value(q->hdfrcv, "tuneop", 0)) {
			/* set tune bit */
			string_appendf(&str, " tune=tune | %d, ", tune);
		} else {
			/* unset tune bit */
			string_appendf(&str, " tune=tune & %d, ", ~tune);
		}
	}

	aid = hash_string(aname);
	ret = aic_cmd_appinfo(q, cd, db);
	if (PROCESS_NOK(ret)) {
		mtc_err("info get failure %d", aid);
		return ret;
	}

	if (!hdf_get_obj(q->hdfsnd, "state")) {
		mtc_warn("%d hasn't regist", aid);
		return REP_ERR_NREGIST;
	}
	pid = hdf_get_int_value(q->hdfsnd, "pid", 0);

	ret = mcs_build_upcol(q->hdfrcv,
						  hdf_get_obj(g_cfg, CONFIG_PATH".UpdateCol.appinfo"), &str);
	if (ret != RET_RBTOP_OK) return REP_ERR_BADPARAM;
	
	MDB_EXEC_EVT(db, NULL, "UPDATE appinfo SET %s WHERE aid=%d;", NULL, str.buf, aid);

	/* TODO memory leak, if exec() failure */
	string_clear(&str);
	
	cache_delf(cd, PREFIX_APPINFO"%d", aid);
	if (pid > 0) {
		cache_delf(cd, PREFIX_APPOUSER"%d_0", pid);
	}

	return REP_OK;
}

/*
 * input : aname(STR)
 * return: NORMAL REP_ERR_NREGIST
 * reply : NULL
 */
static int aic_cmd_appdel(struct queue_entry *q, struct cache *cd, mdb_conn *db)
{
	char *aname;
	int aid, pid = 0, ret;

	REQ_GET_PARAM_STR(q->hdfrcv, "aname", aname);

	aid = hash_string(aname);

	ret = aic_cmd_appinfo(q, cd, db);
	if (PROCESS_NOK(ret)) {
		mtc_err("info get failure %s", aname);
		return ret;
	}

	if (!hdf_get_obj(q->hdfsnd, "state")) {
		mtc_warn("%s not regist", aname);
		return REP_ERR_NREGIST;
	}
	pid = hdf_get_int_value(q->hdfsnd, "pid", 0);

	MDB_EXEC_EVT(db, NULL, "DELETE FROM appinfo WHERE aid=%d;", NULL, aid);
	
	cache_delf(cd, PREFIX_APPINFO"%d", aid);
	if (pid > 0) {
		cache_delf(cd, PREFIX_APPOUSER"%d_0", pid);
	}

	return REP_OK;
}

static int aic_cmd_app_getsecy(struct queue_entry *q, struct cache *cd, mdb_conn *db)
{
	unsigned char *val = NULL; size_t vsize = 0;
	char *aname;
	int aid;

	REQ_GET_PARAM_STR(q->hdfrcv, "aname", aname);
	aid = hash_string(aname);

	if (cache_getf(cd, &val, &vsize, PREFIX_SECY"%d", aid)) {
		unpack_hdf(val, vsize, &q->hdfsnd);
	} else {
		MDB_QUERY_RAW(db, "appinfo", " aname ",
					  " (aid=%d OR pid=%d) AND tune & %d = %d ",
					  NULL, aid, aid, LCS_TUNE_SECY, LCS_TUNE_SECY);
		mdb_set_row(q->hdfsnd, db, " aname ", NULL);
		CACHE_HDF(q->hdfsnd, AIC_CC_SEC, PREFIX_SECY"%d", aid);
	}

	return REP_OK;
}

/*
 * input : pname(STR) aname(STR)
 * return: NORMAL
 * reply : NULL
 */
static int aic_cmd_app_setsecy(struct queue_entry *q, struct cache *cd, mdb_conn *db)
{
	char *pname, *aname;
	int pid, aid, upid;

	REQ_GET_PARAM_STR(q->hdfrcv, "pname", pname);
	REQ_GET_PARAM_STR(q->hdfrcv, "aname", aname);

	pid = hash_string(pname);
	aid = hash_string(aname);

	MDB_EXEC_EVT(db, NULL, "UPDATE appinfo SET tune=tune & %d WHERE aid=%d OR pid=%d "
				 " RETURNING aid", NULL, ~LCS_TUNE_SECY, pid, pid);

	upid = aid;
	mdb_get(db, "i", &upid);
	
	MDB_EXEC_EVT(db, NULL, "UPDATE appinfo SET tune=tune | %d WHERE aid=%d",
				 NULL, LCS_TUNE_SECY, aid);

	cache_delf(cd, PREFIX_APPINFO"%d", upid);
	cache_delf(cd, PREFIX_APPINFO"%d", aid);
	cache_delf(cd, PREFIX_APPOUSER"%d_0", pid);
	cache_delf(cd, PREFIX_SECY"%d", pid);

	return REP_OK;
}

/*
 * input : aname(STR)
 * return: NORMAL
 * reply : [appfoo: ["aid": "222", "aname": "appfoo", ...]
 */
static int aic_cmd_appusers(struct queue_entry *q, struct cache *cd, mdb_conn *db)
{
	unsigned char *val = NULL; size_t vsize = 0;
	int aid;
	char *aname;

	REQ_GET_PARAM_STR(q->hdfrcv, "aname", aname);
	aid = hash_string(aname);
	
	if (cache_getf(cd, &val, &vsize, PREFIX_USERLIST"%d", aid)) {
		unpack_hdf(val, vsize, &q->hdfsnd);
	} else {
		MDB_QUERY_RAW(db, "userinfo", USERINFO_COL,
					  "aid=%d ORDER BY uptime DESC;", NULL, aid);
		mdb_set_rows(q->hdfsnd, db, USERINFO_COL, "userlist", 1);
		CACHE_HDF(q->hdfsnd, AIC_CC_SEC, PREFIX_USERLIST"%d", aid);
	}
	
	return REP_OK;
}

/*
 * input : uname(STR) aname(STR)
 * return: NORMAL REP_ERR_ALREADYJOIN
 * reply : NULL
 */
static int aic_cmd_appuserin(struct queue_entry *q, struct cache *cd, mdb_conn *db)
{
	char *uname, *aname, *ip;
	int aid, ret;

	REQ_GET_PARAM_STR(q->hdfrcv, "uname", uname);
	REQ_GET_PARAM_STR(q->hdfrcv, "aname", aname);
	REQ_GET_PARAM_STR(q->hdfrcv, "ip", ip);
	aid = hash_string(aname);

	ret = aic_cmd_appusers(q, cd, db);
	if (PROCESS_NOK(ret)) {
		mtc_err("userlist get failure %s", aname);
		return ret;
	}

	if (hdf_get_valuef(q->hdfsnd, "userlist.%s.uname", uname)) {
		mtc_warn("%s already join %s", uname, aname);
		return REP_ERR_ALREADYJOIN;
	}

	ret = mdb_exec(db, NULL, "INSERT INTO userinfo (uid, uname, aid, aname, ip) "
				   " VALUES ($1, $2::varchar(256), $3, "
				   " $4::varchar(256), $5::varchar(256));", "isiss",
				   hash_string(uname), uname, aid, aname, ip);
	if (ret != MDB_ERR_NONE) {
		mtc_err("exec failure %s", mdb_get_errmsg(db));
		return REP_ERR_DB;
	}

	/* TODO delete aid's ALL cache */
	cache_delf(cd, PREFIX_USERLIST"%d", aid);

	return REP_OK;
}

/*
 * input : uname(STR) aname(STR)
 * return: NORMAL REP_ERR_NOTJOIN
 * reply : NULL
 */
static int aic_cmd_appuserout(struct queue_entry *q, struct cache *cd, mdb_conn *db)
{
	char *uname, *aname;
	int aid, ret;

	REQ_GET_PARAM_STR(q->hdfrcv, "uname", uname);
	REQ_GET_PARAM_STR(q->hdfrcv, "aname", aname);
	aid = hash_string(aname);

	ret = aic_cmd_appusers(q, cd, db);
	if (PROCESS_NOK(ret)) {
		mtc_err("userlist get failure %s", aname);
		return ret;
	}

	if (!hdf_get_valuef(q->hdfsnd, "userlist.%s.uname", uname)) {
		mtc_warn("%s not join %s", uname, aname);
		return REP_ERR_NOTJOIN;
	}

	ret = mdb_exec(db, NULL, "DELETE FROM userinfo WHERE uid=%d AND aid=%d;",
				   NULL, hash_string(uname), aid);
	if (ret != MDB_ERR_NONE) {
		mtc_err("exec failure %s", mdb_get_errmsg(db));
		return REP_ERR_DB;
	}

	/* TODO delete aid's ALL cache */
	cache_delf(cd, PREFIX_USERLIST"%d", aid);

	return REP_OK;
}

/*
 * input : pname(STR)
 * return: NORMAL
 * reply : [appfoo: ["aid": "293029", "aname": "appfoo", ...]
 */
static int aic_cmd_appousers(struct queue_entry *q, struct cache *cd, mdb_conn *db)
{
	unsigned char *val = NULL; size_t vsize = 0;
	int count, offset;
	int pid;
	char *pname, *aname;

	REQ_GET_PARAM_STR(q->hdfrcv, "pname", pname);
	pid = hash_string(pname);
	
	mmisc_pagediv_get(q->hdfrcv, NULL, &count, &offset);
	
	if (cache_getf(cd, &val, &vsize, PREFIX_APPOUSER"%d_%d", pid, offset)) {
		unpack_hdf(val, vsize, &q->hdfsnd);
	} else {
		MMISC_PAGEDIV_SET_N(q->hdfsnd, offset, db, "appinfo", "pid=%d OR aid=%d",
							NULL, pid, pid);
		MDB_QUERY_RAW(db, "appinfo", APPINFO_COL,
					  "pid=%d OR aid=%d ORDER BY uptime LIMIT %d OFFSET %d",
					  NULL, pid, pid, count, offset);
		mdb_set_rows(q->hdfsnd, db, APPINFO_COL, "users", 1);
		HDF *node = hdf_get_child(q->hdfsnd, "users");
		while (node) {
			/* numcamer */
			aname = hdf_get_value(node, "aname", NULL);
			if (aname) {
				MDB_QUERY_RAW(db, "userinfo", " COUNT(*) AS numcamer ",
							  "aid=%d", NULL, hash_string(aname));
				mdb_set_row(node, db, " numcamer ", NULL);
			}
			node = hdf_obj_next(node);
		}
		CACHE_HDF(q->hdfsnd, AIC_CC_SEC, PREFIX_APPOUSER"%d_%d", pid, offset);
	}
	
	return REP_OK;
}

static void aic_process_driver(struct event_entry *entry, struct queue_entry *q)
{
	struct aic_entry *e = (struct aic_entry*)entry;
	int ret = REP_OK;
	
	mdb_conn *db = e->db;
	struct cache *cd = e->cd;
	struct aic_stats *st = &(e->st);

	st->msg_total++;
	
	mtc_dbg("process cmd %u", q->operation);
	switch (q->operation) {
        CASE_SYS_CMD(q->operation, q, cd, ret);
	case REQ_CMD_APPINFO:
		ret = aic_cmd_appinfo(q, cd, db);
		break;
	case REQ_CMD_APPNEW:
		ret = aic_cmd_appnew(q, cd, db);
		break;
	case REQ_CMD_APPUP:
		ret = aic_cmd_appup(q, cd, db);
		break;
	case REQ_CMD_APPDEL:
		ret = aic_cmd_appdel(q, cd, db);
		break;
	case REQ_CMD_APP_GETSECY:
		ret = aic_cmd_app_getsecy(q, cd, db);
		break;
	case REQ_CMD_APP_SETSECY:
		ret = aic_cmd_app_setsecy(q, cd, db);
		break;
	case REQ_CMD_APPUSERS:
		ret = aic_cmd_appusers(q, cd, db);
		break;
	case REQ_CMD_APPUSERIN:
		ret = aic_cmd_appuserin(q, cd, db);
		break;
	case REQ_CMD_APPUSEROUT:
		ret = aic_cmd_appuserout(q, cd, db);
		break;
	case REQ_CMD_APP_O_USERS:
		ret = aic_cmd_appousers(q, cd, db);
		break;
	case REQ_CMD_STATS:
		st->msg_stats++;
		ret = REP_OK;
		hdf_set_int_value(q->hdfsnd, "msg_total", st->msg_total);
		hdf_set_int_value(q->hdfsnd, "msg_unrec", st->msg_unrec);
		hdf_set_int_value(q->hdfsnd, "msg_badparam", st->msg_badparam);
		hdf_set_int_value(q->hdfsnd, "msg_stats", st->msg_stats);
		hdf_set_int_value(q->hdfsnd, "proc_suc", st->proc_suc);
		hdf_set_int_value(q->hdfsnd, "proc_fai", st->proc_fai);
		break;
	default:
		st->msg_unrec++;
		ret = REP_ERR_UNKREQ;
		break;
	}
	if (PROCESS_OK(ret)) {
		st->proc_suc++;
	} else {
		st->proc_fai++;
		if (ret == REP_ERR_BADPARAM) {
			st->msg_badparam++;
		}
		mtc_err("process %u failed %d", q->operation, ret);
	}
	if (q->req->flags & FLAGS_SYNC) {
		reply_trigger(q, ret);
	}
}

static void aic_stop_driver(struct event_entry *entry)
{
	struct aic_entry *e = (struct aic_entry*)entry;

	/*
	 * e->base.name, e->base will free by mevent_stop_driver() 
	 */
	mdb_destroy(e->db);
	cache_free(e->cd);
}



static struct event_entry* aic_init_driver(void)
{
	struct aic_entry *e = calloc(1, sizeof(struct aic_entry));
	if (e == NULL) return NULL;

	e->base.name = (unsigned char*)strdup(PLUGIN_NAME);
	e->base.ksize = strlen(PLUGIN_NAME);
	e->base.process_driver = aic_process_driver;
	e->base.stop_driver = aic_stop_driver;

	char *dbsn = hdf_get_value(g_cfg, CONFIG_PATH".dbsn", NULL);
	if (mdb_init(&e->db, dbsn) != RET_RBTOP_OK) {
		wlog("init %s failure %s\n", dbsn, mdb_get_errmsg(e->db));
		goto error;
	} else {
		mtc_info("init %s ok", dbsn);
	}
	
	e->cd = cache_create(hdf_get_int_value(g_cfg, CONFIG_PATH".numobjs", 1024), 0);
	if (e->cd == NULL) {
		wlog("init cache failure");
		goto error;
	}
	
	return (struct event_entry*)e;
	
error:
	if (e->base.name) free(e->base.name);
	if (e->db) mdb_destroy(e->db);
	if (e->cd) cache_free(e->cd);
	free(e);
	return NULL;
}

struct event_driver aic_driver = {
	.name = (unsigned char*)PLUGIN_NAME,
	.init_driver = aic_init_driver,
};
