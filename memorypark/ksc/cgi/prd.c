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
        mdb_opfinish_json(ret, cgi->hdf, conn);
        mjson_output_hdf(cgi->hdf, 0);
        goto done;
    } else if (ret == CGI_REQ_DEL) {
        ret = prd_del(cgi->hdf, conn);
        mdb_opfinish_json(ret, cgi->hdf, conn);
        mjson_output_hdf(cgi->hdf, 0);
        goto done;
    } else {
        ret = prd_get(cgi->hdf, conn);
    }
    mdb_opfinish(ret, cgi->hdf, conn, TGT_SELF, URL_CLOSE, false);
    
    /*
     * set template dataset
     */
    hdf_set_value(cgi->hdf, "hdf.loadpaths.local", PATH_FRT_TPL);
    hdf_set_value(cgi->hdf, "Include.content", "prd/list.html");
    hdf_copy(cgi->hdf, PRE_LAYOUT, hdf_get_obj(g_cfg, PRE_LAYOUT));

    hdf_set_value(cgi->hdf, PRE_LAYOUT".title", "Products");
    hdf_set_value(cgi->hdf, PRE_LAYOUT".crumbs.0.name", "Products");
    hdf_set_value(cgi->hdf, PRE_LAYOUT".crumbs.0.href", "/cgi-bin/prd");
    int tid = hdf_get_int_value(cgi->hdf, PRE_QUERY".tid", 0);
    if (tid != 0) {
        char *name = hdf_get_value(cgi->hdf, PRE_OUTPUT".name", NULL);
        hdf_set_value(cgi->hdf, PRE_LAYOUT".crumbs.1.name", name);
        hdf_set_valuef(cgi->hdf, PRE_LAYOUT".crumbs.1.href=/cgi-bin/prd?tid=%d", tid);
    }
    
    HDF *node = hdf_get_obj(cgi->hdf, PRE_LAYOUT".tabs.0");
    while (node) {
        if (!strcmp(hdf_get_value(node, "href", "unknown"), "/cgi-bin/prd")) {
            hdf_set_value(node, "class", "selected");
            break;
        }
        node = hdf_obj_next(node);
    }

#ifdef DEBUG_HDF
    hdf_write_file(cgi->hdf, TC_ROOT"hdf.prd");
#endif

    err = cgi_display(cgi, F_TPL_LAYOUT);
    DIE_NOK_CGI(err);

done:
    mdb_destroy(conn);
    cgi_destroy(&cgi);
    return 0;
}
