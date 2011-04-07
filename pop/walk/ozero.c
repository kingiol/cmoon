#include "mheads.h"
#include "lheads.h"
#include "ozero.h"
#include "oapp.h"

NEOERR* zero_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname;
	NEOERR *err;
	
	APP_CHECK_LOGIN();
	SET_ADMIN_ACTION(evt->hdfrcv, cgi->hdf);
	hdf_copy(cgi->hdf, PRE_OUTPUT".appinfo", evt->hdfrcv);

	return STATUS_OK;
}
