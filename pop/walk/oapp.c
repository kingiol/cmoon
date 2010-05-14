#include "mheads.h"
#include "lheads.h"
#include "oapp.h"

int app_exist_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mdb_conn *conn = (mdb_conn*)hash_lookup(dbh, "main");
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	int ret;
	
	LPRE_DBOP(cgi->hdf, conn, evt);

	HDF *hdf = cgi->hdf;
	
	char *aname = hdf_get_value(hdf, PRE_QUERY".aname", NULL);
	if (!aname) {
		mtc_err("aname NULL");
		return RET_RBTOP_INPUTE;
	}

	hdf_set_int_value(evt->hdfsnd, "aid", (int)hash_string(aname));
	ret = mevent_trigger(evt, aname, REQ_CMD_APPINFO, FLAGS_SYNC);
	if (PROCESS_NOK(ret)) {
		mtc_err("get %s stat failure %d", aname, ret);
		return RET_RBTOP_EVTE;
	}

	if (hdf_get_obj(evt->hdfrcv, "state")) {
		hdf_set_value(hdf, PRE_OUTPUT".exist", "1");
	} else {
		hdf_set_value(hdf, PRE_OUTPUT".exist", "0");
	}
	
	return RET_RBTOP_OK;
}

int app_new_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mdb_conn *conn = (mdb_conn*)hash_lookup(dbh, "main");
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname, *asn, *email;
	int ret;
	
	LPRE_DBOP(cgi->hdf, conn, evt);

	HDF_GET_STR(cgi->hdf, PRE_QUERY".aname", aname);
	HDF_GET_STR(cgi->hdf, PRE_QUERY".asn", asn);
	HDF_GET_STR(cgi->hdf, PRE_QUERY".email", email);

	LEGAL_CK_ANAME(aname);
	LEGAL_CK_EMAIL(email);

	hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));
	mcs_hdf_escape_val(evt->hdfsnd);
	ret = mevent_trigger(evt, aname, REQ_CMD_APPNEW, FLAGS_NONE);

	if (PROCESS_NOK(ret)) {
		mtc_err("set %s info failure", aname, ret);
		return RET_RBTOP_EVTE;
	}

	hdf_set_copy(cgi->hdf, PRE_OUTPUT".aname", PRE_QUERY".aname");
	
	return RET_RBTOP_OK;
}
