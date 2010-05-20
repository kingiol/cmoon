#include "mheads.h"
#include "lcfg.h"

int prd_get(HDF *hdf, mdb_conn *conn)
{
	PRE_DBOP(hdf, conn);

	int tid = hdf_get_int_value(hdf, PRE_QUERY".tid", 0);

	char cols[LEN_SM];
	sprintf(cols, " id, tid, name, url, des ");
	mdb_exec(conn, NULL, "SELECT %s FROM prd WHERE pid=%d "
			 " ORDER BY id DESC LIMIT 20;", NULL, cols);

	mdb_set_rows(hdf, conn, cols, "items");

	hdf_set_value(hdf, "hdf.loadpaths.local", PATH_FRT_TPL);
	hdf_set_value(hdf, "Include.content", "prd/list.html");

	return RET_RBTOP_OK;
}

int prd_add(HDF *hdf, mdb_conn *conn)
{
	PRE_DBOP(hdf, conn);

	int id = 0;
	int pid = hdf_get_int_value(hdf, PRE_QUERY".pid", 0);
	int zid = hdf_get_int_value(hdf, PRE_QUERY".zid", 0);
	char *uname = hdf_get_value(hdf, PRE_QUERY".cname", "");
	char *content = hdf_get_value(hdf, PRE_QUERY".ccontent", "");

	mdb_begin(conn);
	/* prepare input */
	int sn = 0;
	if (pid == 0 && zid == 0) {
		mdb_exec(conn, NULL, "SELECT MAX(sn) FROM prd;", NULL, NULL);
		mdb_get(conn, "i", &sn);
		sn += 1;
	}
	char tm[LEN_TM];
	mmisc_getdatetime(tm, sizeof(tm), "%F %T", 0);

	/* process input */
	mdb_exec(conn, NULL, "INSERT INTO prd (pid, zid, sn, sonCnt, intime, uname, content) "
			 " VALUES (%d, %d, %d, 0, '%s', $1, $2);", "ss", pid, zid, sn, tm, uname, content);
	id = mdb_get_last_id(conn, NULL);
	mdb_exec(conn, NULL, "UPDATE prd set sonCnt=sonCnt+1 WHERE id=%d", NULL, pid);
	mdb_exec(conn, NULL, "UPDATE prd set sonCnt=sonCnt+1 WHERE id=%d", NULL, zid);
	mdb_finish(conn);
	if (mdb_get_errcode(conn) != MDB_ERR_NONE) {
		mtc_err("add prd failure. %s", mdb_get_errmsg(conn));
		return RET_RBTOP_INSERTE;
	}

	/* prepare result */
	hdf_set_int_value(hdf, PRE_OUTPUT".prd.0.id", id);
	hdf_set_copy(hdf, PRE_OUTPUT".prd.0.pid", PRE_QUERY".pid");
	hdf_set_copy(hdf, PRE_OUTPUT".prd.0.zid", PRE_QUERY".zid");
	hdf_set_int_value(hdf, PRE_OUTPUT".prd.0.sn", sn);
	hdf_set_value(hdf, PRE_OUTPUT".prd.0.sonCnt","0");
	hdf_set_value(hdf, PRE_OUTPUT".prd.0.intime", tm);
	hdf_set_copy(hdf, PRE_OUTPUT".prd.0.uname", PRE_QUERY".cname");
	hdf_set_copy(hdf, PRE_OUTPUT".prd.0.content", PRE_QUERY".ccontent");
	
	return RET_RBTOP_OK;
}
