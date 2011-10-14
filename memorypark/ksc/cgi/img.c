#include "mheads.h"
#include "lheads.h"
#include "oimg.h"

HDF *g_cfg;

int main()
{
    CGI *cgi;
    NEOERR *err;
    int ret;

    //sleep(20);
    mtc_init(TC_ROOT"img");
    mconfig_parse_file(SITE_CONFIG, &g_cfg);

    err = cgi_init(&cgi, NULL);
    DIE_NOK_CGI(err);
    err = cgi_parse(cgi);
    DIE_NOK_CGI(err);

    mdb_conn *conn = NULL;
    ret = mdb_init(&conn, DB_DSN);
    mdb_opfinish_json(ret, cgi->hdf, conn);

    ret = img_add(cgi, conn);
    mdb_opfinish_json(ret, cgi->hdf, conn);
    mjson_output_hdf(cgi->hdf, 0);
    
#ifdef DEBUG_HDF
    hdf_write_file(cgi->hdf, TC_ROOT"hdf.img");
#endif

    mdb_destroy(conn);
    cgi_destroy(&cgi);
    return 0;
}
