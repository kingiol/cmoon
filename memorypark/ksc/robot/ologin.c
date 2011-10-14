#include "mheads.h"
#include "lutil.h"
#include "lcfg.h"

static void app_after_login(CGI *cgi, char *uname, char *usn)
{
    char tm[LEN_TM_GMT], *p;

    hdf_set_copy(cgi->hdf, PRE_OUTPUT".uname", PRE_QUERY".uname");
    
    /*
     * set cookie 
     */
    cgi_url_escape(uname, &p);
    cgi_cookie_set(cgi, "uname", p, NULL, SITE_DOMAIN, NULL, 1, 0);
    free(p);

    cgi_url_escape(usn, &p);
    mmisc_getdatetime_gmt(tm, sizeof(tm), "%A, %d-%b-%Y %T GMT", 60*60*3);
    cgi_cookie_set(cgi, "usn", p, NULL, SITE_DOMAIN, tm, 1, 0);
    free(p);
}

int login_get(CGI *cgi, mdb_conn *conn)
{
    char *uname, *usn, *usndb;
    int ret;

    PRE_DBOP(cgi->hdf, conn);

    HDF_GET_STR(cgi->hdf, PRE_QUERY".uname", uname);
    HDF_GET_STR(cgi->hdf, PRE_QUERY".usn", usn);

    mdb_exec(conn, NULL, "SELECT usn FROM user WHERE uname='%s'",
             NULL, uname);

    ret = mdb_get(conn, "s", &usndb);
    if (ret != MDB_ERR_NONE) {
        mtc_err("get password failure, %s", mdb_get_errmsg(conn));
        return RET_RBTOP_SELECTE;
    }

    if (strcmp(usn, usndb)) {
        hdf_set_value(cgi->hdf, PRE_OUTPUT".errmsg", "密码错误");
        return RET_RBTOP_ERROR;
    }

    app_after_login(cgi, uname, usn);
    
    return RET_RBTOP_OK;
}

int login_check(HDF *hdf, mdb_conn *conn)
{
    char *uname, *usn, *usndb;
    int ret;

    PRE_DBOP(hdf, conn);

    HDF_GET_STR(hdf, PRE_COOKIE".uname", uname);
    HDF_GET_STR(hdf, PRE_COOKIE".usn", usn);

    mdb_exec(conn, NULL, "SELECT usn FROM user WHERE uname='%s'",
             NULL, uname);

    ret = mdb_get(conn, "s", &usndb);
    if (ret != MDB_ERR_NONE) {
        mtc_err("get password failure, %s", mdb_get_errmsg(conn));
        return RET_RBTOP_SELECTE;
    }

    if (strcmp(usn, usndb)) {
        return RET_RBTOP_ERROR;
    }

    return RET_RBTOP_OK;
}
