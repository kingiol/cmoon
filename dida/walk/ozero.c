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
    int ftype;
	NEOERR *err;
    
    err = mimg_accept_and_zoomout(cgi, IMG_ROOT, fname, &ftype, 900, 0);
	if (err != STATUS_OK) return nerr_pass(err);
    
    char tok[3] = {0}; strncpy(tok, fname, 2);

    hdf_set_valuef(cgi->hdf, PRE_OUTPUT".imageurl=%s%s/%s.%s",
                   IMG_URL, tok, fname, mimg_type_int2str(ftype));
    hdf_set_valuef(cgi->hdf, PRE_OUTPUT".imageurl_zoom=%s900x0/%s/%s.%s",
                   IMG_URL, tok, fname, mimg_type_int2str(ftype));
    hdf_set_valuef(cgi->hdf, PRE_OUTPUT".imagename=%s.%s",
                   fname, mimg_type_int2str(ftype));

    return STATUS_OK;
}
