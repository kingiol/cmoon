#include "mheads.h"
#include "lcfg.h"
#include "ologin.h"

int feedback_get(HDF *hdf, mdb_conn *conn)
{
    int ret;
    char cols[LEN_SM];
    
    PRE_DBOP(hdf, conn);

    ret = login_check(hdf, conn);
    if (ret == RET_RBTOP_OK) {
        hdf_set_value(hdf, PRE_OUTPUT".admin", "1");
        /*
         * get feedbacks
         */
        sprintf(cols, " id, contact, content ");
        mdb_exec(conn, NULL, "SELECT %s FROM feedback WHERE 1 "
                 " ORDER BY id LIMIT 12;", NULL, cols);
        mdb_set_rows(hdf, conn, cols, PRE_OUTPUT".items");
    }
    
    return RET_RBTOP_OK;
}

int feedback_add(HDF *hdf, mdb_conn *conn)
{
    PRE_DBOP(hdf, conn);

    int ret;
    char *contact, *content;

    HDF_GET_STR(hdf, PRE_QUERY".contact", contact);
    HDF_GET_STR(hdf, PRE_QUERY".content", content);

    ret = mdb_exec(conn, NULL, "INSERT INTO feedback (contact, content) "
                   " VALUES ($1, $2)", "ss", contact, content);
    if (ret != MDB_ERR_NONE) {
        mtc_err("db failure %s", mdb_get_errmsg(conn));
        return RET_RBTOP_INSERTE;
    }
    
    return RET_RBTOP_OK;
}

int feedback_del(HDF *hdf, mdb_conn *conn)
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

    ret = mdb_exec(conn, NULL, "DELETE FROM feedback WHERE id=%d", NULL, id);
    if (ret != MDB_ERR_NONE) {
        mtc_err("db failure %s", mdb_get_errmsg(conn));
        return RET_RBTOP_DELETEE;
    }
    
    return RET_RBTOP_OK;
}
