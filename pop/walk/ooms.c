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
	if (PROCESS_NOK(mevent_trigger(evt, ip.buf, REQ_CMD_PLACEGET, FLAGS_SYNC))) {
		mtc_err("get %s stat failure %d", ip.buf, evt->errcode);
		string_clear(&ip);
		return;
	}

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

int oms_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mdb_conn *conn = (mdb_conn*)hash_lookup(dbh, "main");
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname;
	int ret;
	
	/*
	 * input check
	 */
	LPRE_DBOP(cgi->hdf, conn, evt);
	
	HDF_GET_STR_IDENT(cgi->hdf, PRE_COOKIE".aname", aname);
	LEGAL_CK_ANAME(aname);

	/*
	 * want sth seriously
	 */
	ret = app_check_login_data_get(cgi, dbh, evth, ses);
	if (ret != RET_RBTOP_OK) {
		mtc_warn("doesn't login, %d", ret);
		return RET_RBTOP_NOTLOGIN;
	}
	hdf_copy(cgi->hdf, PRE_OUTPUT".appinfo", evt->hdfrcv);
	
	/*
	 * prepare data 
	 */
	hdf_set_value(evt->hdfsnd, "aname", aname);

	
	/*
	 * trigger
	 */
	//hdf_destroy(&evt->hdfrcv);
	if (PROCESS_NOK(mevent_trigger(evt, aname, REQ_CMD_APPUSERS, FLAGS_SYNC))) {
		mtc_err("get %s userlist failure %d", aname, evt->errcode);
		return RET_RBTOP_EVTE;
	}
	hdf_copy(cgi->hdf, PRE_OUTPUT".userlist", evt->hdfrcv);

	ips2places(hdf_get_obj(cgi->hdf, PRE_OUTPUT".userlist"), evth);
	
	
	return RET_RBTOP_OK;
}

int oms_edit_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	int ret = oms_data_get(cgi, dbh, evth, ses);

	int tune = hdf_get_int_value(cgi->hdf, PRE_OUTPUT".appinfo.tune", 0);

	if (tune & LCS_TUNE_QUIET)
		hdf_set_value(cgi->hdf, PRE_OUTPUT".appinfo.quiet", "1");
	if (tune & LCS_TUNE_SMS)
		hdf_set_value(cgi->hdf, PRE_OUTPUT".appinfo.sms", "1");

	return ret;
}

int oms_edit_data_mod(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mdb_conn *conn = (mdb_conn*)hash_lookup(dbh, "main");
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname;
	int ret;
	
	/*
	 * want sth seriously
	 */
	ret = app_check_login_data_get(cgi, dbh, evth, ses);
	if (ret != RET_RBTOP_OK) {
		mtc_warn("doesn't login, %d", ret);
		return RET_RBTOP_NOTLOGIN;
	}
	
	/*
	 * input check
	 */
	LPRE_DBOP(cgi->hdf, conn, evt);
	
	HDF_GET_STR_IDENT(cgi->hdf, PRE_COOKIE".aname", aname);
	LEGAL_CK_ANAME(aname);

	/*
	 * prepare data 
	 */
	hdf_set_value(evt->hdfsnd, "aname", aname);
	hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));
	mcs_hdf_escape_val(evt->hdfsnd);
	
	/*
	 * trigger
	 */
	if (PROCESS_NOK(mevent_trigger(evt, aname, REQ_CMD_APPUP, FLAGS_SYNC))) {
		mtc_err("process %s failure %d", aname, evt->errcode);
		return RET_RBTOP_EVTE;
	}
	
	return RET_RBTOP_OK;
}

int oms_users_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mdb_conn *conn = (mdb_conn*)hash_lookup(dbh, "main");
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname;
	int ret;
	
	/*
	 * input check
	 */
	LPRE_DBOP(cgi->hdf, conn, evt);
	
	HDF_GET_STR_IDENT(cgi->hdf, PRE_COOKIE".aname", aname);
	LEGAL_CK_ANAME(aname);

	/*
	 * want sth seriously
	 */
	ret = app_check_login_data_get(cgi, dbh, evth, ses);
	if (ret != RET_RBTOP_OK) {
		mtc_warn("doesn't login, %d", ret);
		return RET_RBTOP_NOTLOGIN;
	}
	hdf_copy(cgi->hdf, PRE_OUTPUT".appinfo", evt->hdfrcv);
	
	/*
	 * prepare data 
	 */
	hdf_set_value(evt->hdfsnd, "pname", aname);
	
	/*
	 * trigger
	 */
	if (PROCESS_NOK(mevent_trigger(evt, aname, REQ_CMD_APP_O_USERS, FLAGS_SYNC))) {
		mtc_err("process %s failure %d", aname, evt->errcode);
		return RET_RBTOP_EVTE;
	}
	hdf_copy(cgi->hdf, PRE_OUTPUT".users", evt->hdfrcv);
	
	return RET_RBTOP_OK;
}

int oms_users_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mdb_conn *conn = (mdb_conn*)hash_lookup(dbh, "main");
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname, *pname, *email;
	int ret;
	
	/*
	 * input check
	 */
	LPRE_DBOP(cgi->hdf, conn, evt);
	
	HDF_GET_STR_IDENT(cgi->hdf, PRE_COOKIE".aname", pname);
	HDF_GET_STR(cgi->hdf, PRE_QUERY".aname", aname);
	HDF_GET_STR(cgi->hdf, PRE_QUERY".email", email);
	LEGAL_CK_ANAME(pname);
	LEGAL_CK_ANAME(aname);
	LEGAL_CK_EMAIL(email);

	/*
	 * want sth seriously
	 */
	ret = app_check_login_data_get(cgi, dbh, evth, ses);
	if (ret != RET_RBTOP_OK) {
		mtc_warn("doesn't login, %d", ret);
		return RET_RBTOP_NOTLOGIN;
	}
	
	if (hdf_get_int_value(evt->hdfrcv, "pid", 1) != 0) {
		mtc_warn("%s want add %s", pname, aname);
		return RET_RBTOP_LIMITE;
	}

	int limit = hdf_get_int_value(g_cfg, "Limit.max_users_per_freesite", 5);
	if (hdf_get_int_value(evt->hdfrcv, "numuser", 0) >= limit) {
		mtc_warn("%s want to add more users %s", pname, aname);
		return RET_RBTOP_NEEDVIP;
	}
	
	/*
	 * prepare data 
	 */
	hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));
	hdf_set_value(evt->hdfsnd, "pname", pname);
	hdf_set_value(evt->hdfsnd, "aname", aname);
	hdf_set_value(evt->hdfsnd, "state", hdf_get_value(evt->hdfrcv, "state", LCS_ST_FREE));
	hdf_set_value(evt->hdfsnd, "masn", aname);
	
	/*
	 * trigger
	 */
	if (PROCESS_NOK(mevent_trigger(evt, aname, REQ_CMD_APPNEW, FLAGS_SYNC))) {
		mtc_err("process %s failure %d", aname, evt->errcode);
		if (evt->errcode == REP_ERR_ALREADYREGIST)
			return RET_RBTOP_EXISTE;
		return RET_RBTOP_EVTE;
	}
	
	return RET_RBTOP_OK;
}

int oms_users_data_del(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mdb_conn *conn = (mdb_conn*)hash_lookup(dbh, "main");
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname, *pname;
	int ret;
	
	/*
	 * input check
	 */
	LPRE_DBOP(cgi->hdf, conn, evt);
	
	HDF_GET_STR_IDENT(cgi->hdf, PRE_COOKIE".aname", pname);
	HDF_GET_STR(cgi->hdf, PRE_QUERY".aname", aname);
	LEGAL_CK_ANAME(pname);
	LEGAL_CK_ANAME(aname);

	/*
	 * want sth seriously
	 */
	ret = app_check_login_data_get(cgi, dbh, evth, ses);
	if (ret != RET_RBTOP_OK) {
		mtc_warn("doesn't login, %d", ret);
		return RET_RBTOP_NOTLOGIN;
	}

	if (!strcmp(pname, aname)) {
		mtc_warn("%s want kill him self", pname);
		return RET_RBTOP_LIMITE;
	}
	
	hdf_set_value(evt->hdfsnd, "pname", pname);
	if (PROCESS_NOK(mevent_trigger(evt, aname, REQ_CMD_APP_O_USERS, FLAGS_SYNC))) {
		mtc_err("get %s userlist failure %d", aname, evt->errcode);
		return RET_RBTOP_EVTE;
	}
	if (!hdf_get_obj(evt->hdfrcv, aname)) {
		mtc_warn("%s not %s's", aname, pname);
		return RET_RBTOP_LIMITE;
	}

	/*
	 * prepare data 
	 */
	hdf_set_value(evt->hdfsnd, "aname", aname);
	
	/*
	 * trigger
	 */
	if (PROCESS_NOK(mevent_trigger(evt, aname, REQ_CMD_APPDEL, FLAGS_SYNC))) {
		mtc_err("process %s failure %d", aname, evt->errcode);
		if (evt->errcode == REP_ERR_NREGIST)
			return RET_RBTOP_NEXIST;
		return RET_RBTOP_EVTE;
	}
	
	return RET_RBTOP_OK;
}
