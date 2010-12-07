#include "mheads.h"
#include "lheads.h"
#include "ooms.h"

#include "oapp.h"

static void ips2places(HDF *hdf, HASH *evth)
{
	if (!hdf || !evth) return;

	mevent_t *evt = (mevent_t*)hash_lookup(evth, "place");
	if (!evt) {
		mtc_err("can't find place event");
		return;
	}

	char *p, *q;
	HDF *node, *rnode;
	STRING ip;

	string_init(&ip);
	node = hdf_obj_child(hdf);
	while (node) {
		p = hdf_get_value(node, "ip", NULL);
		if (p) string_appendf(&ip, "%s,", p);
		
		node = hdf_obj_next(node);
	}

	hdf_set_value(evt->hdfsnd, "ip", ip.buf);
	/* TODO memroy leak string not clear on NOK */
	MEVENT_TRIGGER_VOID(evt, ip.buf, REQ_CMD_PLACEGET, FLAGS_SYNC);

	if (evt->hdfrcv) {
		node = hdf_obj_child(hdf);
		rnode = hdf_obj_child(evt->hdfrcv);
		while (node && rnode) {
			p = hdf_get_value(node, "ip", NULL);
			if (p) {
				q = hdf_get_value(rnode, "ip", NULL);
				while (!q && rnode) {
					q = hdf_get_value(rnode, "ip", NULL);
					rnode = hdf_obj_next(rnode);
				}

				if (!q) break;
				
				if (!strcmp(p, q)) {
					hdf_set_value(node, "city", hdf_get_value(rnode, "c", NULL));
					hdf_set_value(node, "area", hdf_get_value(rnode, "a", NULL));
					rnode = hdf_obj_next(rnode);
				}
			}
			
			node = hdf_obj_next(node);
		}
	}
	
	string_clear(&ip);
}

NEOERR* oms_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname;
	NEOERR *err;
	
	APP_CHECK_LOGIN();
	
	hdf_copy(cgi->hdf, PRE_OUTPUT".appinfo", evt->hdfrcv);
	if (hdf_get_int_value(evt->hdfrcv, "pid", 1) == 0) {
		hdf_set_value(cgi->hdf, PRE_SPECIAL_ACTION".0", "actions_1");
	}

	/*
	 * prepare data 
	 */
	hdf_set_value(evt->hdfsnd, "aname", aname);

	
	/*
	 * trigger
	 */
	MEVENT_TRIGGER(evt, aname, REQ_CMD_APPUSERS, FLAGS_SYNC);
	hdf_copy(cgi->hdf, PRE_OUTPUT, evt->hdfrcv);
	ips2places(hdf_get_obj(cgi->hdf, PRE_OUTPUT".userlist"), evth);
	
	return STATUS_OK;
}

NEOERR* oms_camer_data_del(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname, *uname;
	NEOERR *err;

	APP_CHECK_LOGIN();
	
	HDF_GET_STR(cgi->hdf, PRE_QUERY".uname", uname);
	LEGAL_CK_ANAME(uname);

	hdf_set_value(evt->hdfsnd, "aname", aname);
	hdf_set_value(evt->hdfsnd, "uname", uname);

	MEVENT_TRIGGER(evt, aname, REQ_CMD_APPUSEROUT, FLAGS_NONE);

	evt = (mevent_t*)hash_lookup(evth, "msg");
	if (evt) {
		hdf_set_value(evt->hdfsnd, "name", uname);
		hdf_set_value(evt->hdfsnd, "name2", aname);
		MEVENT_TRIGGER(evt, uname, REQ_CMD_DEL_BOTH, FLAGS_NONE);
	}

	return STATUS_OK;
}

NEOERR* oms_edit_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	NEOERR *err = oms_data_get(cgi, dbh, evth, ses);

	int tune = hdf_get_int_value(cgi->hdf, PRE_OUTPUT".appinfo.tune", 0);

	if (tune & LCS_TUNE_QUIET)
		hdf_set_value(cgi->hdf, PRE_OUTPUT".appinfo.quiet", "1");
	if (tune & LCS_TUNE_SMS)
		hdf_set_value(cgi->hdf, PRE_OUTPUT".appinfo.sms", "1");

	return nerr_pass(err);
}

NEOERR* oms_edit_data_mod(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname;
	NEOERR *err;
	
	APP_CHECK_LOGIN();
	
	/*
	 * prepare data 
	 */
	hdf_set_value(evt->hdfsnd, "aname", aname);
	hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));
	
	/*
	 * trigger
	 */
	MEVENT_TRIGGER(evt, aname, REQ_CMD_APPUP, FLAGS_NONE);
	
	return STATUS_OK;
}

NEOERR* oms_users_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname;
	NEOERR *err;
	
	APP_CHECK_LOGIN();
	
	hdf_copy(cgi->hdf, PRE_OUTPUT".appinfo", evt->hdfrcv);
	
	/*
	 * prepare data 
	 */
	hdf_set_value(evt->hdfsnd, "pname", aname);
	
	/*
	 * trigger
	 */
	MEVENT_TRIGGER(evt, aname, REQ_CMD_APP_O_USERS, FLAGS_SYNC);
	hdf_copy(cgi->hdf, PRE_OUTPUT, evt->hdfrcv);
	
	return STATUS_OK;
}

NEOERR* oms_users_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname, *pname, *email;
	NEOERR *err;
	
	APP_CHECK_LOGIN();
	
	HDF_GET_STR_IDENT(cgi->hdf, PRE_COOKIE".aname", pname);
	HDF_GET_STR(cgi->hdf, PRE_QUERY".aname", aname);
	HDF_GET_STR(cgi->hdf, PRE_QUERY".email", email);
	LEGAL_CK_ANAME(pname);
	LEGAL_CK_ANAME(aname);
	LEGAL_CK_EMAIL(email);

	if (hdf_get_int_value(evt->hdfrcv, "pid", 1) != 0)
		return nerr_raise(LERR_LIMIT, "%s want add %s", pname, aname);

	int limit = hdf_get_int_value(g_cfg, "Limit.max_users_per_freesite", 5);
	if (hdf_get_int_value(evt->hdfrcv, "numuser", 0) >= limit)
		return nerr_raise(LERR_NEEDVIP, "%s want to add more users %s", pname, aname);
	
	/*
	 * prepare data 
	 */
	hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));
	hdf_set_value(evt->hdfsnd, "pname", pname);
	hdf_set_value(evt->hdfsnd, "aname", aname);
	hdf_set_int_value(evt->hdfsnd, "state",
					  hdf_get_int_value(evt->hdfrcv, "state", LCS_ST_FREE));
	hdf_set_value(evt->hdfsnd, "masn", aname);
	
	/*
	 * trigger
	 */
	if (PROCESS_NOK(mevent_trigger(evt, aname, REQ_CMD_APPNEW, FLAGS_SYNC))) {
		//mtc_err("process %s failure %d", aname, evt->errcode);
		if (evt->errcode == REP_ERR_ALREADYREGIST)
			return nerr_raise(LERR_EXIST, "%s already exist", aname);
		char *zpa = NULL;
		hdf_write_string(evt->hdfrcv, &zpa);
		return nerr_raise(LERR_MEVENT, "pro %s %d failure %d %s",
						  evt->ename, REQ_CMD_APPNEW, evt->errcode, zpa);
	}
	
	return STATUS_OK;
}

NEOERR* oms_users_data_del(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname, *pname;
	NEOERR *err;
	
	APP_CHECK_LOGIN();

	HDF_GET_STR_IDENT(cgi->hdf, PRE_COOKIE".aname", pname);
	HDF_GET_STR(cgi->hdf, PRE_QUERY".aname", aname);
	LEGAL_CK_ANAME(pname);
	LEGAL_CK_ANAME(aname);

	if (!strcmp(pname, aname))
		return nerr_raise(LERR_LIMIT, "%s want kill him self", pname);
	
	hdf_set_value(evt->hdfsnd, "pname", pname);
	MEVENT_TRIGGER(evt, aname, REQ_CMD_APP_O_USERS, FLAGS_SYNC);
	if (!hdf_get_valuef(evt->hdfrcv, "users.%s.aname", aname))
		return nerr_raise(LERR_LIMIT, "%s not %s 's admin", pname, aname);

	/*
	 * prepare data 
	 */
	hdf_set_value(evt->hdfsnd, "aname", aname);
	
	/*
	 * trigger
	 */
	if (PROCESS_NOK(mevent_trigger(evt, aname, REQ_CMD_APPDEL, FLAGS_SYNC))) {
		if (evt->errcode == REP_ERR_NREGIST)
			return nerr_raise(LERR_NEXIST, "%s don't regist", aname);
		char *zpa = NULL;
		hdf_write_string(evt->hdfrcv, &zpa);
		return nerr_raise(LERR_MEVENT, "pro %s %d failure %d %s",
						  evt->ename, REQ_CMD_APPDEL, evt->errcode, zpa);
	}
	
	return STATUS_OK;
}

NEOERR* oms_secy_data_mod(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname, *uname;
	NEOERR *err;

	HDF_GET_STR(cgi->hdf, PRE_QUERY".aname", uname);
	
	APP_CHECK_LOGIN_ADMIN(uname);

	hdf_set_value(evt->hdfsnd, "pname", aname);
	hdf_set_value(evt->hdfsnd, "aname", uname);

	MEVENT_TRIGGER(evt, aname, REQ_CMD_APP_SETSECY, FLAGS_NONE);

	return STATUS_OK;
}
