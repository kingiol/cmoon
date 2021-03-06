#include "mheads.h"
#include "lcfg.h"
#include "ologin.h"

int prd_get(HDF *hdf, mdb_conn *conn)
{
    PRE_DBOP(hdf, conn);

    int tid = hdf_get_int_value(hdf, PRE_QUERY".tid", 0);

    if (login_check(hdf, conn) == RET_RBTOP_OK) {
        hdf_set_value(hdf, PRE_OUTPUT".admin", "1");
        hdf_set_value(hdf, PRE_INCLUDE".res", "prd/list.res.html");
    }

    char cols[LEN_SM];
    hdf_set_int_value(hdf, PRE_OUTPUT".tid", tid);

    /*
     * get tid's name 
     */
    char *name;
    mdb_exec(conn, NULL, "SELECT name FROM prd WHERE id=%d;", NULL, tid);
    mdb_get(conn, "s", &name);
    hdf_set_value(hdf, PRE_OUTPUT".name", name);
    
    /*
     * get directories 
     */
    sprintf(cols, " id, tid, name ");
    mdb_exec(conn, NULL, "SELECT %s FROM prd WHERE type=0;", NULL, cols);
    mdb_set_rows(hdf, conn, cols, PRE_OUTPUT".dirs");

    
    /*
     * get products
     */
    sprintf(cols, " id, tid, name, url, des ");
    if (tid != 0) {
        mdb_exec(conn, NULL, "SELECT %s FROM prd WHERE type=1 AND "
                 " tid=%d ORDER BY id LIMIT 12;",
                 NULL, cols, tid);
    } else {
        mdb_exec(conn, NULL, "SELECT %s FROM prd WHERE type=1 "
                 " ORDER BY id LIMIT 12;",
                 NULL, cols);
    }
    mdb_set_rows(hdf, conn, cols, PRE_OUTPUT".items");

    return RET_RBTOP_OK;
}

int prd_add(HDF *hdf, mdb_conn *conn)
{
    PRE_DBOP(hdf, conn);

    int tid, ret;
    char *name, *url, *des;

    ret = login_check(hdf, conn);
    if (ret != RET_RBTOP_OK) {
        mtc_err("user auth failure %d", ret);
        hdf_set_value(hdf, PRE_OUTPUT".errmsg", "请登录后操作");
        return RET_RBTOP_ERROR;
    }
    
    HDF_GET_INT(hdf, PRE_QUERY".tid", tid);
    HDF_GET_STR(hdf, PRE_QUERY".name", name);
    HDF_GET_STR(hdf, PRE_QUERY".url", url);
    HDF_GET_STR(hdf, PRE_QUERY".des", des);

    ret = mdb_exec(conn, NULL, "INSERT INTO prd (type, tid, name, url, des) "
                   " VALUES (1, $1, $2, $3, $4)", "isss", tid, name, url, des);
    if (ret != MDB_ERR_NONE) {
        mtc_err("db failure %s", mdb_get_errmsg(conn));
        return RET_RBTOP_UPDATEE;
    }
    
    return RET_RBTOP_OK;
}

int prd_del(HDF *hdf, mdb_conn *conn)
{
    PRE_DBOP(hdf, conn);

    int id, ret;

    ret = login_check(hdf, conn);
    if (ret != RET_RBTOP_OK) {
        mtc_err("user auth failure %d", ret);
        hdf_set_value(hdf, PRE_OUTPUT".errmsg", "请登录后操作");
        return RET_RBTOP_ERROR;
    }
    
    HDF_GET_INT(hdf, PRE_QUERY".id", id);

    ret = mdb_exec(conn, NULL, "DELETE FROM prd WHERE id=%d", NULL, id);
    if (ret != MDB_ERR_NONE) {
        mtc_err("db failure %s", mdb_get_errmsg(conn));
        return RET_RBTOP_DELETEE;
    }
    
    return RET_RBTOP_OK;
}
