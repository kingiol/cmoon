#include "mheads.h"
#include "lheads.h"
#include "ooms.h"

#include "oapp.h"

int oms_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
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
	
	HDF_GET_STR(cgi->hdf, PRE_COOKIE".aname", aname);
	LEGAL_CK_ANAME(aname);

	/*
	 * prepare data 
	 */
	hdf_set_value(evt->hdfsnd, "aname", aname);

	
	/*
	 * trigger
	 */
	if (PROCESS_NOK(mevent_trigger(evt, aname, REQ_CMD_USERLIST, FLAGS_SYNC))) {
		mtc_err("get %s userlist failure %d", aname, evt->errcode);
		return RET_RBTOP_EVTE;
	}
	hdf_copy(cgi->hdf, PRE_OUTPUT".userlist", evt->hdfrcv);
	
	
	return RET_RBTOP_OK;
}
