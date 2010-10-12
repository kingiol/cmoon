#include "mheads.h"
#include "lheads.h"
#include "ookol.h"

#include "oapp.h"

int okol_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mdb_conn *conn = (mdb_conn*)hash_lookup(dbh, "main");
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname;
	
	LPRE_DBOP(cgi->hdf, conn);
	
	APP_CHECK_LOGIN();
	
	return RET_RBTOP_OK;
}
