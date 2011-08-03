#include "mheads.h"
#include "lcfg.h"

int detail_get(HDF *hdf, mdb_conn *conn)
{
    char *tid, *name, *url, *des;
    
    PRE_DBOP(hdf, conn);

    int id = hdf_get_int_value(hdf, PRE_QUERY".id", 0);

    char cols[LEN_SM];
    hdf_set_int_value(hdf, PRE_OUTPUT".id", id);

    /*
     * get directories 
     */
    sprintf(cols, " id, id, name ");
    mdb_exec(conn, NULL, "SELECT %s FROM detail WHERE type=0;", NULL, cols);
    mdb_set_rows(hdf, conn, cols, PRE_OUTPUT".dirs");

    
    /*
     * get products
     */
    sprintf(cols, " tid, name, url, des ");
    mdb_exec(conn, NULL, "SELECT %s FROM detail WHERE id=%d;",
             NULL, cols, id);
    if (mdb_get(conn, "ssss", &tid, &name, &url, &des) != MDB_ERR_NONE) {
        mtc_err("%d failure %s", id, mdb_get_errmsg(conn));
        return RET_RBTOP_SELECTE;
    }

    hdf_set_int_value(hdf, PRE_OUTPUT".item.id", id);
    hdf_set_value(hdf, PRE_OUTPUT".item.tid", tid);
    hdf_set_value(hdf, PRE_OUTPUT".item.name", name);
    hdf_set_value(hdf, PRE_OUTPUT".item.url", url);
    hdf_set_value(hdf, PRE_OUTPUT".item.des", des);
        
    return RET_RBTOP_OK;
}
