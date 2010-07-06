#include "mheads.h"
#include "lheads.h"
#include "oblog.h"

static int blog_file_get(char *f, CGI *cgi, int sn)
{
	struct stat s;
	char *c, *content, *p, *q;
	char fname[LINE_MAX], prekey[64], intime[64];
	NEOERR *err;
	
	if (!f || !cgi) return RET_RBTOP_INPUTE;
	
	snprintf(fname, sizeof(fname), "%s%s", PATH_BLOG, f);
	if (stat(fname, &s) == -1) {
		mtc_warn("stat file %s failure %s", fname, strerror(errno));
		return RET_RBTOP_OPENFILE;
	}
	
	struct tm *stm = gmtime(&(s.st_mtime));
	strftime(intime, sizeof(intime), "%F %T", stm);
		
	err = ne_load_file(fname, &c);
	RETURN_V_NOK(err, RET_RBTOP_OPENFILE);
	mcs_text_escape(c, &content);

	p = q = NULL;
	p = strstr(content, "<br /><br />");
	if (p) {
		*p = '\0';
		p += strlen("<br /><br />");
	}

	if (sn < 0) {
		strcpy(prekey, "blog");
	} else {
		sprintf(prekey, "blogs.%d", sn);
	}
	
	hdf_set_valuef(cgi->hdf, PRE_OUTPUT".%s.fname=%s", prekey, f);
	hdf_set_valuef(cgi->hdf, PRE_OUTPUT".%s.title=%s", prekey, content);
	hdf_set_valuef(cgi->hdf, PRE_OUTPUT".%s.content=%s", prekey, p ? p: content);
	hdf_set_valuef(cgi->hdf, PRE_OUTPUT".%s.intime=%s", prekey, intime);
		
	if (p) {
		q = strstr(p, "<br /><br />");
		if (q) *q = '\0';
	}
	hdf_set_valuef(cgi->hdf, PRE_OUTPUT".%s.abs=%s", prekey, p ? p: content);
	/* q not null, blog hasn't finish */
	if (q) {
		hdf_set_valuef(cgi->hdf, PRE_OUTPUT".%s.done=0", prekey);
	} else {
		hdf_set_valuef(cgi->hdf, PRE_OUTPUT".%s.done=1", prekey);
	}
		
	free(c);
	free(content);
	return RET_RBTOP_OK;
}

static int blog_index_get(CGI *cgi)
{
	FILE *fp;
	char line[LINE_MAX];

	if ((fp = fopen(BLOG_INDEX, "r")) == NULL) {
		mtc_err("open index file %s failure", BLOG_INDEX);
		return RET_RBTOP_OPENFILE;
	}

	int i = 0, x;
	while (fgets(line, LINE_MAX, fp) != NULL) {
		
		/* rubish, shit, fuck... */
		x = strlen(line) -1;
		while (x > 0 && (line[x] == '\n')) line[x--] = '\0';

		if (blog_file_get(line, cgi, i) == RET_RBTOP_OK) i++;
	}
	
	fclose(fp);
	return RET_RBTOP_OK;
}

int blog_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	if (!cgi || !cgi->hdf) return RET_RBTOP_HDFNINIT;

	char *f = hdf_get_value(cgi->hdf, PRE_QUERY".f", NULL);
	if (f != NULL) {
		return blog_file_get(f, cgi, -1);
	} else {
		return blog_index_get(cgi);
	}

#if 0	
	hdf_set_value(cgi->hdf, PRE_OUTPUT".blogs.0.title", "日志1");
	hdf_set_value(cgi->hdf, PRE_OUTPUT".blogs.0.content", "xxdfpsdfsd<br />哈阿");
	hdf_set_value(cgi->hdf, PRE_OUTPUT".blogs.0.intime", "2010-07-03 12:22");
	hdf_set_value(cgi->hdf, PRE_OUTPUT".blogs.0.fname", "rizhi1.txt");
	hdf_set_value(cgi->hdf, PRE_OUTPUT".blogs.0.count_comment", "20");
#endif
}
