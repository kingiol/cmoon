#include "mheads.h"
#include "lutil.h"
#include "lcfg.h"

int img_add(CGI *cgi, mdb_conn *conn)
{
	unsigned char hash[LEN_MD5];
	int ret;

	PRE_DBOP(cgi->hdf, conn);
	
	FILE *fp = cgi_filehandle(cgi, "imagename");
	if (fp == NULL) {
		mtc_err("input file named: imagename not found");
		return RET_RBTOP_INPUTE;
	}

	ret = lutil_image_accept(fp, "prd", hash);
	if (ret != RET_RBTOP_OK) {
		mtc_err("accept image failure %d", ret);
		return ret;
	}
	
	hdf_set_valuef(cgi->hdf, PRE_OUTPUT".imageurl=/img/prd/%s/%s.jpg",
                   IMG_ORI, hash);
	hdf_set_valuef(cgi->hdf, PRE_OUTPUT".imagename=%s.jpg", hash);
	return RET_RBTOP_OK;
}
