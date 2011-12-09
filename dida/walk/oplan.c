#include "mheads.h"
#include "lheads.h"
#include "oplan.h"

NEOERR* plan_match_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "plan");
    
    if (!cgi || !cgi->hdf || !evt) return nerr_raise(NERR_ASSERT, "paramter null");

    hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_PLAN_GET_BY_GEO, FLAGS_SYNC);

    hdf_copy(cgi->hdf, PRE_OUTPUT, evt->hdfrcv);
    hdf_set_attr(cgi->hdf, PRE_OUTPUT".plans", "type", "array");

    /*
     * fuck the time
     */

    return STATUS_OK;
}
