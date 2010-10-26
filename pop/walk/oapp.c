#include "mheads.h"
#include "lheads.h"
#include "oapp.h"

static void app_after_login(CGI *cgi, char *aname, char *masn)
{
	char tm[LEN_TM_GMT], *p;

	hdf_set_copy(cgi->hdf, PRE_OUTPUT".aname", PRE_QUERY".aname");
	
	/*
	 * set cookie 
	 */
	neos_url_escape(aname, &p, NULL);
	cgi_cookie_set(cgi, "aname", p, NULL, SITE_DOMAIN, NULL, 1, 0);
	free(p);

	//cgi_url_escape(masn, &p);
	mmisc_getdatetime_gmt(tm, sizeof(tm), "%A, %d-%b-%Y %T GMT", 60*60*3);
	cgi_cookie_set(cgi, "masn", masn, NULL, SITE_DOMAIN, tm, 1, 0);
}

int app_exist_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname;
	
	/*
	 * input check
	 */
	LPRE_EVTOP(cgi->hdf, evt);

	HDF_GET_STR(cgi->hdf, PRE_QUERY".aname", aname);

	/*
	 * prepare data 
	 */
	hdf_set_value(evt->hdfsnd, "aname", aname);

	
	/*
	 * trigger
	 */
	MEVENT_TRIGGER(RET_RBTOP_EVTE, evt, aname, REQ_CMD_APPINFO, FLAGS_SYNC);

	/*
	 * set output
	 */
	if (hdf_get_obj(evt->hdfrcv, "state")) {
		hdf_set_value(cgi->hdf, PRE_OUTPUT".exist", "1");
		hdf_set_value(cgi->hdf, PRE_OUTPUT".msg", "站点名已被占用");
	} else {
		hdf_set_value(cgi->hdf, PRE_OUTPUT".exist", "0");
	}
	
	return RET_RBTOP_OK;
}

int app_new_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname, *asn, *email, masn[LEN_CK];
	
	/*
	 * input check
	 */
	LPRE_EVTOP(cgi->hdf, evt);
	
	HDF_GET_STR(cgi->hdf, PRE_QUERY".aname", aname);
	HDF_GET_STR(cgi->hdf, PRE_QUERY".asn", asn);
	HDF_GET_STR(cgi->hdf, PRE_QUERY".email", email);
	
	LEGAL_CK_ANAME(aname);
	LEGAL_CK_EMAIL(email);

	/*
	 * prepare data 
	 */
	hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));
	// mcs_hdf_escape_val(evt->hdfsnd);
	
	hdf_set_int_value(evt->hdfsnd, "state", LCS_ST_FREE);
	
	memset(masn, 0x0, sizeof(masn));
	mcs_rand_string(masn, sizeof(masn));
	hdf_set_value(evt->hdfsnd, "masn", masn);

	/*
	 * trigger
	 */
	MEVENT_TRIGGER(RET_RBTOP_EVTE, evt, aname, REQ_CMD_APPNEW, FLAGS_SYNC);

	/*
	 * follow-up
	 */
	app_after_login(cgi, aname, masn);
	
	return RET_RBTOP_OK;
}

int app_login_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname, *asn, masn[LEN_CK];
	
	/*
	 * input check
	 */
	LPRE_EVTOP(cgi->hdf, evt);
	
	HDF_GET_STR(cgi->hdf, PRE_QUERY".aname", aname);
	HDF_GET_STR(cgi->hdf, PRE_QUERY".asn", asn);
	
	LEGAL_CK_ANAME(aname);
	
	/*
	 * prepare data 
	 */
	hdf_set_value(evt->hdfsnd, "aname", aname);
	
	/*
	 * trigger
	 */
	MEVENT_TRIGGER(RET_RBTOP_EVTE, evt, aname, REQ_CMD_APPINFO, FLAGS_SYNC);
	
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
			mcs_rand_string(masn, sizeof(masn));
			hdf_set_value(evt->hdfsnd, "aname", aname);
			hdf_set_value(evt->hdfsnd, "masn", masn);
			mevent_trigger(evt, aname, REQ_CMD_APPUP, FLAGS_NONE);
			app_after_login(cgi, aname, masn);
			return RET_RBTOP_OK;
		}
	}
	
	return RET_RBTOP_LOGINPSW;
}

int app_logout_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname;
	
	APP_CHECK_LOGIN();

	HDF_GET_STR_IDENT(cgi->hdf, PRE_COOKIE".aname", aname);
	
	hdf_set_value(evt->hdfsnd, "aname", aname);
	hdf_set_value(evt->hdfsnd, "masn", "0");

	MEVENT_TRIGGER(RET_RBTOP_EVTE, evt, aname, REQ_CMD_APPUP, FLAGS_NONE);

	return RET_RBTOP_OK;
}

int app_check_login_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname, *masn;
	
	/*
	 * input check
	 */
	LPRE_EVTOP(cgi->hdf, evt);

	HDF_GET_STR_IDENT(cgi->hdf, PRE_COOKIE".aname", aname);
	HDF_GET_STR_IDENT(cgi->hdf, PRE_COOKIE".masn", masn);
	LEGAL_CK_ANAME(aname);

	/*
	 * prepare data 
	 */
	hdf_set_value(evt->hdfsnd, "aname", aname);

	
	/*
	 * trigger
	 */
	MEVENT_TRIGGER(RET_RBTOP_EVTE, evt, aname, REQ_CMD_APPINFO, FLAGS_SYNC);

	/*
	 * verify
	 */
	char *masndb = hdf_get_value(evt->hdfrcv, "masn", NULL);
	if (masndb) {
		if (!strcmp(masndb, masn)) {
			//app_after_login(cgi, aname, masn);
			return RET_RBTOP_OK;
		}
	}
	mtc_warn("%s<====>%s", masndb, masn);

	return RET_RBTOP_NOTLOGIN;
}
