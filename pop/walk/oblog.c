#include "mheads.h"
#include "lheads.h"
#include "oblog.h"

static int blog_index_get(CGI *cgi)
{
	FILE *fp;
	struct stat s;
	char line[LINE_MAX], fname[LINE_MAX], intime[64];
	char *c, *content, *p, *q;
	NEOERR *err;

	if ((fp = fopen(BLOG_INDEX, "r")) == NULL) {
		mtc_err("open index file %s failure", BLOG_INDEX);
		return RET_RBTOP_OPENFILE;
	}

	int i = 0;
	while (fgets(line, LINE_MAX, fp) != NULL) {
		/* rubish, shit, fuck... */
		p = line;
		while (*p != '\0') {
		  if (*p == '\n') {
		    *p = '\0';
		    break;
		  }
		  p++;
		}
		
	  	p = q = NULL;
		snprintf(fname, sizeof(fname), "%s%s", PATH_BLOG, line);

		if (stat(fname, &s) == -1) {
		        mtc_warn("stat file %s failure %s", fname, strerror(errno));
			continue;
		}
		struct tm *stm = gmtime(&(s.st_mtime));
		strftime(intime, sizeof(intime), "%F %T", stm);
		
		err = ne_load_file(fname, &c);
		CONTINUE_NOK(err);
		mcs_text_escape(c, &content);

		p = strstr(content, "<br /><br />");
		if (p) {
			*p = '\0';
			p += strlen("<br /><br />");
		}
		
		hdf_set_valuef(cgi->hdf, PRE_OUTPUT".blogs.%d.fname=%s", i, line);
		hdf_set_valuef(cgi->hdf, PRE_OUTPUT".blogs.%d.title=%s", i, content);
		hdf_set_valuef(cgi->hdf, PRE_OUTPUT".blogs.%d.content=%s", i, p ? p: content);
		hdf_set_valuef(cgi->hdf, PRE_OUTPUT".blogs.%d.intime=%s", i, intime);
		hdf_set_valuef(cgi->hdf, PRE_OUTPUT".blogs.%d.count_comment=%d", i, 0);
		
		if (p) {
		  q = strstr(p, "<br /><br />");
		  if (q) *q = '\0';
		}
		hdf_set_valuef(cgi->hdf, PRE_OUTPUT".blogs.%d.abs=%s", i, p ? p: content);
		
		free(c);
		free(content);
		i++;
	}
	
	fclose(fp);
	return RET_RBTOP_OK;
}

int blog_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	if (!cgi || !cgi->hdf) return RET_RBTOP_HDFNINIT;

	return blog_index_get(cgi);

#if 0	
	hdf_set_value(cgi->hdf, PRE_OUTPUT".blogs.0.title", "日志1");
	hdf_set_value(cgi->hdf, PRE_OUTPUT".blogs.0.content", "xxdfpsdfsd<br />哈阿");
	hdf_set_value(cgi->hdf, PRE_OUTPUT".blogs.0.intime", "2010-07-03 12:22");
	hdf_set_value(cgi->hdf, PRE_OUTPUT".blogs.0.fname", "rizhi1.txt");
	hdf_set_value(cgi->hdf, PRE_OUTPUT".blogs.0.count_comment", "20");
#endif
}
