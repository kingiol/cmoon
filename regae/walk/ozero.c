#include "mheads.h"
#include "lheads.h"
#include "ozero.h"

NEOERR* zero_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "aic");

    if (!cgi || !cgi->hdf || !evt) return nerr_raise(NERR_ASSERT, "paramter null");

    return STATUS_OK;
}

NEOERR* zero_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "aic");

    if (!cgi || !cgi->hdf || !evt) return nerr_raise(NERR_ASSERT, "paramter null");

    return STATUS_OK;
}

NEOERR* zero_img_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    char hash[LEN_MD5];
    FILE *fp = cgi_filehandle(cgi, NULL);
	NEOERR *err;

    MCS_NOT_NULLB(cgi->hdf, fp);

    err = lutil_image_accept(fp, hash);
	if (err != STATUS_OK) return nerr_pass(err);

    char tok[3];
    strncpy(tok, hash, 2);
    tok[2] = '\0';
    
    hdf_set_valuef(cgi->hdf, PRE_OUTPUT".imageurl=%s%s/%s.jpg", IMG_URL, tok, hash);
    hdf_set_valuef(cgi->hdf, PRE_OUTPUT".imagename=%s.jpg", hash);

    return STATUS_OK;
}
