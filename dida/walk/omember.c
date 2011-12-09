#include "mheads.h"
#include "lheads.h"
#include "omember.h"

NEOERR* member_info_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "member");
    
    if (!cgi || !cgi->hdf || !evt) return nerr_raise(NERR_ASSERT, "paramter null");

    hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_MEMBER_GET, FLAGS_SYNC);

    hdf_copy(cgi->hdf, PRE_OUTPUT, evt->hdfrcv);

    return STATUS_OK;
}
