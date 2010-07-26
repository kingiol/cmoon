#include "mheads.h"
#include "lheads.h"

#include "oapp.h"

/*
 * TODO how make local dlsym ok? so tired 
 */
static void lutil_donotcall()
{
	app_exist_data_get(NULL, NULL, NULL, NULL);
	oms_data_get(NULL, NULL, NULL, NULL);
	blog_data_get(NULL, NULL, NULL, NULL);
	place_data_get(NULL, NULL, NULL, NULL);
}

void* lutil_get_data_handler(void *lib, CGI *cgi, session_t *ses)
{
	char *hname, *tp;
	void *res;

	hname = ses->dataer;
	res = dlsym(lib, hname);
	if ((tp = dlerror()) != NULL) {
		mtc_err("%s", tp);
		return NULL;
	} else
		mtc_info("%s found for data handler", hname);
	return res;
}

int lutil_fetch_count(HDF *hdf, mdb_conn *conn, char *table, char *cond)
{
	PRE_DBOP(hdf, conn);
    if (table == NULL || cond == NULL)
        return RET_RBTOP_INPUTE;

    char *buf;
    size_t datalen;
    int count = 0;
    
    buf = mmc_getf(&datalen, 0, PRE_MMC_COUNT".%s.%s", table, cond);
    if (buf == NULL) {
        mdb_exec(conn, NULL, "SELECT count(*) FROM %s WHERE %s;",
                 NULL, table, cond);
        mdb_get(conn, "s", &buf);
        count = atoi(buf);
        mmc_storef(MMC_OP_SET, (void*)buf, 0, ONE_HOUR, 0,
                   PRE_MMC_COUNT".%s.%s", table, cond);
    } else {
        count = atoi(buf);
    }

    hdf_set_int_value(hdf, PRE_OUTPUT".ttnum", count);

    return RET_RBTOP_OK;
}

int lutil_fetch_countf(HDF *hdf, mdb_conn *conn, char *table, char *cfmt, ...)
{
    char cond[LEN_SM];
    va_list ap;

    va_start(ap, cfmt);
    vsnprintf(cond, sizeof(cond), cfmt, ap);
    va_end(ap);

    return lutil_fetch_count(hdf, conn, table, cond);
}
