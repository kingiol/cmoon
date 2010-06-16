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
	 * input check
	 */
	LPRE_DBOP(cgi->hdf, conn, evt);

	ret = app_check_login_data_get(cgi, dbh, evth, ses);
	if (ret != RET_RBTOP_OK) {
		mtc_warn("doesn't login, %d", ret);
		return RET_RBTOP_NOTLOGIN;
	}
	
	HDF_GET_STR(cgi->hdf, PRE_COOKIE".aname", aname);
	//LEGAL_CK_ANAME(aname);
	
	return RET_RBTOP_OK;
}
