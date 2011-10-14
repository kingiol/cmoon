#include "mheads.h"
#include "lheads.h"
#include "ozero.h"

NEOERR* zero_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    if (!cgi || !cgi->hdf) return nerr_raise(NERR_ASSERT, "paramter null");

    return STATUS_OK;
}

NEOERR* zero_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    if (!cgi || !cgi->hdf) return nerr_raise(NERR_ASSERT, "paramter null");

    return STATUS_OK;
}
