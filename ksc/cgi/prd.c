#include "mheads.h"
#include "lheads.h"
#include "oprd.h"

HDF *g_cfg;

int main()
{
	CGI *cgi;
	NEOERR *err;
	int ret;

	//sleep(20);
	mtc_init(TC_ROOT"prd");
	mconfig_parse_file(SITE_CONFIG, &g_cfg);

	err = cgi_init(&cgi, NULL);
	DIE_NOK_CGI(err);
	err = cgi_parse(cgi);
	DIE_NOK_CGI(err);

	mdb_conn *conn = NULL;
	ret = mdb_init(&conn, DB_DSN);
	mdb_opfinish(ret, cgi->hdf, conn, TGT_SELF, URL_CLOSE, false);

	ret = CGI_REQ_METHOD(cgi);
	if (ret == CGI_REQ_POST) {
		ret = prd_add(cgi->hdf, conn);
	} else {
		ret = prd_get(cgi->hdf, conn);
	}
	mdb_opfinish(ret, cgi->hdf, conn, TGT_SELF, URL_CLOSE, false);
	
#ifdef DEBUG_HDF
	hdf_write_file(cgi->hdf, TC_ROOT"hdf.prd");
#endif

	err = cgi_display(cgi, F_TPL_LAYOUT);
	DIE_NOK_CGI(err);

	mdb_destroy(conn);
	cgi_destroy(&cgi);
	return 0;
}
