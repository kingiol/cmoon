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

NEOERR* zero_img_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    char fname[LEN_MD5];
	NEOERR *err;
    
    err = mimg_accept_and_zoomout(cgi, IMG_ROOT, fname, 900, 0);
	if (err != STATUS_OK) return nerr_pass(err);
    
    char tok[3] = {0}; strncpy(tok, fname, 2);

    hdf_set_valuef(cgi->hdf, PRE_OUTPUT".imageurl=%s%s/%s.jpg", IMG_URL, tok, fname);
    hdf_set_valuef(cgi->hdf, PRE_OUTPUT".imageurl_zoom=%s900x0/%s/%s.jpg",
                   IMG_URL, tok, fname);
    hdf_set_valuef(cgi->hdf, PRE_OUTPUT".imagename=%s.jpg", fname);

    return STATUS_OK;
}
