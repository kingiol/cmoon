#include "mheads.h"
#include "lheads.h"
#include "oblog.h"

int blog_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	struct dirent **eps = NULL;
	int n;
	
	if (!cgi || !cgi->hdf) return RET_RBTOP_HDFNINIT;

	n = scandir(PATH_BLOG, &eps, NULL, blog_sorter);
	for (int i = 0; i < n; i++) {
		mtc_dbg("parse file %s", eps[i]->d_name);

		free(eps[i]);
	}
	if (n > 0) free(eps);

	
	hdf_set_value(cgi->hdf, PRE_OUTPUT".blogs.0.title", "日志1");
	hdf_set_value(cgi->hdf, PRE_OUTPUT".blogs.0.content", "xxdfpsdfsd<br />哈阿");
	hdf_set_value(cgi->hdf, PRE_OUTPUT".blogs.0.intime", "2010-07-03 12:22");
	hdf_set_value(cgi->hdf, PRE_OUTPUT".blogs.0.fname", "rizhi1.txt");
	hdf_set_value(cgi->hdf, PRE_OUTPUT".blogs.0.count_comment", "20");
	
	return RET_RBTOP_OK;
}
