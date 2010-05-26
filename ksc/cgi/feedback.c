#include "mheads.h"
#include "lheads.h"
#include "ofeedback.h"

HDF *g_cfg;

int main()
{
	CGI *cgi;
	NEOERR *err;
	int ret;

	//sleep(20);
	mtc_init(TC_ROOT"feedback");
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
		ret = feedback_add(cgi->hdf, conn);
		mdb_opfinish_json(ret, cgi->hdf, conn);
		mjson_output_hdf(cgi->hdf, 0);
		goto done;
	} else if (ret == CGI_REQ_DEL) {
		ret = feedback_del(cgi->hdf, conn);
		mdb_opfinish_json(ret, cgi->hdf, conn);
		mjson_output_hdf(cgi->hdf, 0);
		goto done;
	}
	
	ret = feedback_get(cgi->hdf, conn);
	mdb_opfinish(ret, cgi->hdf, conn, TGT_SELF, URL_CLOSE, false);
	
	/*
	 * set template dataset
	 */
	hdf_set_value(cgi->hdf, "hdf.loadpaths.local", PATH_FRT_TPL);
	hdf_set_value(cgi->hdf, "Include.content", "feedback.htm");
	hdf_copy(cgi->hdf, PRE_LAYOUT, hdf_get_obj(g_cfg, PRE_LAYOUT));

	hdf_set_value(cgi->hdf, PRE_LAYOUT".title", "Feedback");
	hdf_set_value(cgi->hdf, PRE_LAYOUT".crumbs.0.name", "Feedback");
	hdf_set_value(cgi->hdf, PRE_LAYOUT".crumbs.0.href", "/cgi-bin/feedback");
	
	HDF *node = hdf_get_obj(cgi->hdf, PRE_LAYOUT".tabs.0");
	while (node) {
		if (!strcmp(hdf_get_value(node, "href", "unknown"), "/cgi-bin/feedback")) {
			hdf_set_value(node, "class", "selected");
			break;
		}
		node = hdf_obj_next(node);
	}

#ifdef DEBUG_HDF
	hdf_write_file(cgi->hdf, TC_ROOT"hdf.feedback");
#endif

	err = cgi_display(cgi, F_TPL_LAYOUT);
	DIE_NOK_CGI(err);

done:
	mdb_destroy(conn);
	cgi_destroy(&cgi);
	return 0;
}
