#include "mheads.h"
#include "lheads.h"
#include "omsg.h"

int msg_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mdb_conn *conn = (mdb_conn*)hash_lookup(dbh, "main");
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "msg");
	//char *from, *to;

	LPRE_DBOP(cgi->hdf, conn, evt);
	
	//HDF_GET_STR(cgi->hdf, PRE_QUERY".from", from);
	//HDF_GET_STR(cgi->hdf, PRE_QUERY".to", to);
	//hdf_set_value(evt->hdfsnd, "from", from);
	//hdf_set_value(evt->hdfsnd, "to", to);

	hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));
	MEVENT_TRIGGER(RET_RBTOP_EVTE, evt, NULL, REQ_CMD_SAYWITHOTHER, FLAGS_SYNC);

	hdf_copy(cgi->hdf, PRE_OUTPUT, evt->hdfrcv);
	hdf_set_attr(cgi->hdf, PRE_OUTPUT".raws", "type", "array");

	return RET_RBTOP_OK;
}
