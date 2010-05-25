#include "mheads.h"
#include "lheads.h"

HDF *g_cfg = NULL;

static int tpl_page(const struct dirent *ent)
{
	if (reg_search(".*.html$", ent->d_name))
		return 1;
	else
		return 0;
}

int main(int argc, char *argv[])
{
	CSPARSE *cs = NULL;
	HDF *hdf = NULL, *cnode, *pnode;
	NEOERR *err;
	struct dirent **eps = NULL;
	char *href, fin[_POSIX_PATH_MAX], fout[_POSIX_PATH_MAX];
	STRING str;
	int n;

	mconfig_parse_file(SITE_CONFIG, &g_cfg);
	mtc_init(TC_ROOT"gen");

	/*
	 * process every template file in tpl dir
	 */
	n = scandir(PATH_FRT_TPL, &eps, tpl_page, alphasort);
	for (int i = 0; i < n; i++) {
		strncpy(fin, eps[i]->d_name, sizeof(fin));
		free(eps[i]);
		if (strstr(F_TPL_LAYOUT, fin)) continue;
		mtc_dbg("parse file %s", fin);

		cs = NULL; hdf = NULL;
		string_init(&str);

		err = hdf_init(&hdf);
		JUMP_NOK(err, next);
		hdf_copy(hdf, NULL, g_cfg);

		/*
		 * set layout hdf
		 */
		cnode = hdf_get_obj(hdf, "Layout.tabs.0");
		pnode = hdf_get_obj(hdf, "Layout");
		while (cnode) {
			href = hdf_get_value(cnode, "href", NULL);
			if (href && strstr(href, fin)) {
				hdf_set_value(cnode, "class", "selected");
				hdf_set_value(pnode, "title", hdf_get_value(cnode, "name", NULL));
				hdf_copy(pnode, "crumbs.0", cnode);
			}
			
			cnode = hdf_obj_next(cnode);
		}
		
		snprintf(fout, sizeof(fout), "%s%s", PATH_FRT_DOC, fin);

		/*
		 * set content hdf
		 */
		hdf_set_valuef(hdf, PRE_INCLUDE".content=%s%s", PATH_FRT_TPL, fin);
		

		/*
		 * rend
		 */
#ifdef DEBUG_HDF
		hdf_write_file(hdf, TC_ROOT"hdf.gen");
#endif
		
		err = cs_init(&cs, hdf);
		JUMP_NOK(err, next);

		err = cs_parse_file(cs, F_TPL_LAYOUT);
		JUMP_NOK(err, next);

		string_init(&str);
		err = cs_render(cs, &str, mcs_strcb);
		DIE_NOK_MTL(err);

		if(!mcs_str2file(str, fout)) {
			mtc_err("write result to out file failure");
		} else {
			mtc_dbg("write %s ok", fout);
		}

	next:
		cs_destroy(&cs);
		hdf_destroy(&hdf);
		string_clear(&str);
	}

	if (n > 0) free(eps);

	return 0;
}
