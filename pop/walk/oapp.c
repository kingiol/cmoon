#include "mheads.h"
#include "lheads.h"
#include "oapp.h"

static void app_after_login(CGI *cgi, int aid, char *aname, char *masn)
{
	char tok[64], tm[LEN_TM_GMT], *p;

	hdf_set_copy(cgi->hdf, PRE_OUTPUT".aname", PRE_QUERY".aname");
	
	/*
	 * set cookie 
	 */
	sprintf(tok, "%d", aid);
	cgi_cookie_set(cgi, "aid", tok, NULL, SITE_DOMAIN, NULL, 1, 0);

	cgi_url_escape(aname, &p);
	cgi_cookie_set(cgi, "aname", p, NULL, SITE_DOMAIN, NULL, 1, 0);
	free(p);

	cgi_url_escape(masn, &p);
	mmisc_getdatetime_gmt(tm, sizeof(tm), "%A, %d-%b-%Y %T GMT", 60*60*3);
	cgi_cookie_set(cgi, "masn", p, NULL, SITE_DOMAIN, tm, 1, 0);
	free(p);
}

int app_exist_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	
	mdb_conn *conn = (mdb_conn*)hash_lookup(dbh, "main");
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname;
	int ret;
	
	/*
	 * input check
	 */
	LPRE_DBOP(cgi->hdf, conn, evt);

	HDF_GET_STR(cgi->hdf, PRE_QUERY".aname", aname);

	/*
	 * prepare data 
	 */
	hdf_set_int_value(evt->hdfsnd, "aid", hash_string(aname));

	
	/*
	 * trigger
	 */
	ret = mevent_trigger(evt, aname, REQ_CMD_APPINFO, FLAGS_SYNC);
	if (PROCESS_NOK(ret)) {
		mtc_err("get %s stat failure %d", aname, ret);
		return RET_RBTOP_EVTE;
	}

	/*
	 * set output
	 */
	if (hdf_get_obj(evt->hdfrcv, "state")) {
		hdf_set_value(cgi->hdf, PRE_OUTPUT".exist", "1");
	} else {
		hdf_set_value(cgi->hdf, PRE_OUTPUT".exist", "0");
	}
	
	return RET_RBTOP_OK;
}

int app_new_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mdb_conn *conn = (mdb_conn*)hash_lookup(dbh, "main");
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname, *asn, *email, masn[LEN_CK];
	int aid, ret;
	
	/*
	 * input check
	 */
	LPRE_DBOP(cgi->hdf, conn, evt);
	
	HDF_GET_STR(cgi->hdf, PRE_QUERY".aname", aname);
	HDF_GET_STR(cgi->hdf, PRE_QUERY".asn", asn);
	HDF_GET_STR(cgi->hdf, PRE_QUERY".email", email);
	
	LEGAL_CK_ANAME(aname);
	LEGAL_CK_EMAIL(email);

	/*
	 * prepare data 
	 */
	hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));
	mcs_hdf_escape_val(evt->hdfsnd);
	
	aid = (int)hash_string(aname);
	hdf_set_int_value(evt->hdfsnd, "aid", aid);
	hdf_set_int_value(evt->hdfsnd, "state", LCS_ST_FREE);
	
	memset(masn, 0x0, sizeof(masn));
	neo_rand_string(masn, sizeof(masn));
	hdf_set_value(evt->hdfsnd, "masn", masn);

	/*
	 * trigger
	 */
	ret = mevent_trigger(evt, aname, REQ_CMD_APPNEW, FLAGS_SYNC);
	if (PROCESS_NOK(ret)) {
		mtc_err("set %s info failure", aname, ret);
		if (ret == REP_ERR_ALREADYREGIST)
			hdf_set_value(cgi->hdf, PRE_ERRMSG, "站点标志已被他人注册过了");
		return RET_RBTOP_EVTE;
	}

	/*
	 * follow-up
	 */
	app_after_login(cgi, aid, aname, masn);
	
	return RET_RBTOP_OK;
}

int app_login_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mdb_conn *conn = (mdb_conn*)hash_lookup(dbh, "main");
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname, *asn, masn[LEN_CK];
	int aid, ret;
	
	/*
	 * input check
	 */
	LPRE_DBOP(cgi->hdf, conn, evt);
	
	HDF_GET_STR(cgi->hdf, PRE_QUERY".aname", aname);
	HDF_GET_STR(cgi->hdf, PRE_QUERY".asn", asn);
	
	LEGAL_CK_ANAME(aname);
	
	/*
	 * prepare data 
	 */
	aid = (int)hash_string(aname);
	hdf_set_int_value(evt->hdfsnd, "aid", aid);
	
	/*
	 * trigger
	 */
	ret = mevent_trigger(evt, aname, REQ_CMD_APPINFO, FLAGS_SYNC);
	if (PROCESS_NOK(ret)) {
		mtc_err("get %s stat failure %d", aname, ret);
		return RET_RBTOP_EVTE;
	}
	
	/*
	 * verify
	 */
	char *asndb = hdf_get_value(evt->hdfrcv, "asn", NULL);
	if (asndb) {
		if (!strcmp(asndb, asn)) {
			/*
			 * follow-up
			 */
			memset(masn, 0x0, sizeof(masn));
			neo_rand_string(masn, sizeof(masn));
			hdf_set_int_value(evt->hdfsnd, "aid", aid);
			hdf_set_value(evt->hdfsnd, "masn", masn);
			mevent_trigger(evt, aname, REQ_CMD_APPUP, FLAGS_NONE);
			app_after_login(cgi, aid, aname, masn);
			return RET_RBTOP_OK;
		}
	}
	
	return RET_RBTOP_LOGINPSW;
}

int app_check_login_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mdb_conn *conn = (mdb_conn*)hash_lookup(dbh, "main");
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname, *masn;
	int aid, ret;
	
	/*
	 * input check
	 */
	LPRE_DBOP(cgi->hdf, conn, evt);

	HDF_GET_INT(cgi->hdf, PRE_QUERY".aid", aid);
	HDF_GET_STR(cgi->hdf, PRE_QUERY".aname", aname);
	HDF_GET_STR(cgi->hdf, PRE_QUERY".masn", masn);

	if (aid != (int)hash_string(aname)) {
		mtc_warn("%d %s don't match", aid, aname);
		return RET_RBTOP_NOTLOGIN;
	}

	/*
	 * prepare data 
	 */
	hdf_set_int_value(evt->hdfsnd, "aid", aid);

	
	/*
	 * trigger
	 */
	ret = mevent_trigger(evt, aname, REQ_CMD_APPINFO, FLAGS_SYNC);
	if (PROCESS_NOK(ret)) {
		mtc_err("get %s stat failure %d", aname, ret);
		return RET_RBTOP_EVTE;
	}

	/*
	 * verify
	 */
	char *masndb = hdf_get_value(evt->hdfrcv, "masn", NULL);
	if (masndb) {
		if (!strcmp(masndb, masn)) {
			app_after_login(cgi, aid, aname, masn);
			return RET_RBTOP_OK;
		}
	}

	return RET_RBTOP_NOTLOGIN;
}
