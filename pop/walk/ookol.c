#include "mheads.h"
#include "lheads.h"
#include "ookol.h"

#include "oapp.h"

#define STAT_COL " type, count, " \
	" ceil(date_part('epoch', intime)*1000) as intime "
//	"to_char(intime, 'YYYY-MM-DD') as intime "

int okol_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	mdb_conn *conn = (mdb_conn*)hash_lookup(dbh, "mtool");
	mevent_t *evt = (mevent_t*)hash_lookup(evth, "aic");
	char *aname;
	STRING str;
	
	LPRE_DBOP(cgi->hdf, conn);
	
	APP_CHECK_LOGIN();

	if (hdf_get_int_value(evt->hdfrcv, "state", 0) != LCS_ST_ROOT) {
		mtc_warn("%s wan't be root", aname);
		return RET_RBTOP_LIMITE;
	}

	/*
	 * build query conditon
	 */
	string_init(&str);
	mcs_build_querycond_s(hdf_get_obj(cgi->hdf, PRE_QUERY),
						  hdf_get_child(g_cfg, "Db.QueryCond.stat.s"),
						  &str);
	mcs_build_querycond_i(hdf_get_obj(cgi->hdf, PRE_QUERY),
						  hdf_get_child(g_cfg, "Db.QueryCond.stat.i"),
						  &str);
	if (str.len <= 0) string_append(&str, " intime > current_date - 7 AND type=0 ");
	else string_append(&str, " 1=1 ");

	/*
	 * execute
	 */
	MDB_QUERY_RAW(conn, "stat", STAT_COL, "%s ORDER BY intime", NULL, str.buf);
	string_clear(&str);
	mdb_set_rows(cgi->hdf, conn, STAT_COL, PRE_OUTPUT".sts", 0);
	hdf_set_attr(cgi->hdf, PRE_OUTPUT".sts", "type", "array");
	
	return RET_RBTOP_OK;
}
