#include "mheads.h"
#include "lheads.h"
#include "oblog.h"

HDF *g_cfg = NULL;

static void rend_node(HASH *tplh, HDF *node)
{
	CSPARSE *cs = NULL;
	HDF *dhdf;
	STRING str;
	NEOERR *err;
	char *fname, tok[_POSIX_PATH_MAX];

	if (!node) return;
	
	cs = (CSPARSE*)hash_lookup(tplh, "blog");
	dhdf = (HDF*)hash_lookup(tplh, "blog_hdf");
	if (!cs || !dhdf) return;
	
	hdf_copy(node, NULL, dhdf);
	ltpl_prepare_rend(node, "layout.html");
	hdf_set_copy(node, PRE_LAYOUT".title", PRE_OUTPUT".blog.title");

	fname = hdf_get_value(node, PRE_OUTPUT".blog.fname", NULL);
	if (fname == NULL) {
		fname = "index";
		hdf_set_value(node, PRE_LAYOUT".title", "开物博客");
	}
	
	cs->hdf = node;

	string_init(&str);
	
	err = cs_render(cs, &str, mcs_strcb);
	RETURN_NOK(err);
	
	snprintf(tok, sizeof(tok), "%s%s.html", PATH_BLOG_H, fname);
	mutil_makesure_dir(tok);
	if (!mcs_str2file(str, tok)) {
		mtc_err("write file %s failure", tok);
	}

	cs->hdf = NULL;
	string_clear(&str);
}

int main(int argc, char *argv[])
{
	mconfig_parse_file(SITE_CONFIG, &g_cfg);
	mtc_init(TC_ROOT"blogs");

	HDF *hdf, *node, *pnode;
	HASH *tplh;

	hdf_init(&hdf);
	ltpl_init(&tplh);

	blog_index_get(hdf);
	
#ifdef DEBUG_HDF
	hdf_write_file(hdf, TC_ROOT"hdf.blogs");
#endif

	rend_node(tplh, hdf);

	node = hdf_get_obj(hdf, PRE_OUTPUT".blogs.0");
	while (node) {
		hdf_init(&pnode);
		hdf_copy(pnode, PRE_OUTPUT".blog", node);
		rend_node(tplh, pnode);

		hdf_destroy(&pnode);
		node = hdf_obj_next(node);
	}

	hdf_destroy(&hdf);
	ltpl_destroy(tplh);
	return 0;
}
