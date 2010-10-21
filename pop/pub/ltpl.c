#include "mheads.h"
#include "lheads.h"

static int tpl_config(const struct dirent *ent)
{
	if (reg_search(".*.hdf", ent->d_name))
		return 1;
	else
		return 0;
}

void ltpl_prepare_rend(HDF *hdf, char *tpl)
{
	char key[LEN_ST];
	HDF *tmphdf; 
	if (hdf == NULL) return;

	/*
	 * set template variable
	 */
    hdf_set_copy(hdf, PRE_OUT_TPL".uri", PRE_REQ_URI_RW);
    
	/*
	 * merge dataset from g_cfg 
	 */
	snprintf(key, sizeof(key), PRE_CFG_DATASET".%s", tpl);
	tmphdf = hdf_get_obj(g_cfg, key);
	if (tmphdf != NULL) hdf_copy(hdf, PRE_CFG_LAYOUT, tmphdf);
	
	/*
	 * special actions
	 */
	tmphdf = hdf_get_child(hdf, PRE_SPECIAL_ACTION);
	while (tmphdf) {
		snprintf(key, sizeof(key), PRE_LAYOUT".%s", hdf_obj_value(tmphdf));
		hdf_copy(hdf, PRE_LAYOUT".actions", hdf_get_obj(hdf, key));
		
		tmphdf = hdf_obj_next(tmphdf);
	}
	
	/*
	 * set classes
	 */
	char *tab = hdf_get_value(hdf, "Layout.tabpart", NULL);
	if (tab) {
		hdf_set_valuef(hdf, "Layout.tabs.%s.class=selected", tab);
	}
}

int ltpl_parse_file(HASH *dbh, void *lib, char *dir, char *name, HASH *outhash)
{
	char *tp = NULL, *tpl = NULL, *dataer = NULL;
	HDF *node = NULL, *dhdf = NULL, *child = NULL;
	CSPARSE *cs = NULL;
	STRING str;
	NEOERR *err;
	char fname[_POSIX_PATH_MAX], tok[64];
	int (*data_handler)(HDF *hdf, HASH *dbh);
	int ret;
	
	memset(fname, 0x0, sizeof(fname));
	snprintf(fname, sizeof(fname), "%s/%s", dir, name);
	hdf_init(&node);
	err = hdf_read_file(node, fname);
	RETURN_V_NOK(err, RET_RBTOP_ERROR);

	child = hdf_obj_child(node);
	while (child != NULL) {
		mtc_dbg("parse node %s", hdf_obj_name(child));
		string_init(&str);
		err = cs_init(&cs, hdf_get_obj(child, PRE_CFG_DATASET));
		JUMP_NOK(err, wnext);
			
		hdf_set_value(cs->hdf, "hdf.loadpaths.local", dir);

		err = cgi_register_strfuncs(cs);
		JUMP_NOK(err, wnext);
		mcs_register_bitop_functions(cs);
		tpl = hdf_get_value(child, PRE_CFG_LAYOUT, "null.html");
		snprintf(fname, sizeof(fname), "%s/%s", PATH_TPL, tpl);
		err = cs_parse_file(cs, fname);
		JUMP_NOK(err, wnext);

		if (outhash != NULL) {
			/*
			 * strdup the key, baby, because we'll free the hdf later
			 */
			hash_insert(outhash, (void*)strdup(hdf_obj_name(child)), (void*)cs);
			if (hdf_get_obj(child, PRE_CFG_DATASET)) {
				hdf_init(&dhdf);
				hdf_copy(dhdf, NULL, hdf_get_obj(child, PRE_CFG_DATASET));
				snprintf(tok, sizeof(tok), "%s_hdf", hdf_obj_name(child));
				hash_insert(outhash, (void*)strdup(tok), (void*)dhdf);
			}
		}
			
		if (hdf_get_value(child, PRE_CFG_OUTPUT, NULL) != NULL) {
			ltpl_prepare_rend(hdf_get_obj(child, PRE_CFG_DATASET), tpl);
				
			/*
			 * get_data
			 */
			dataer = hdf_get_value(child, PRE_CFG_DATAER, NULL);
			if (dataer != NULL && lib) {
				data_handler = dlsym(lib, dataer);
				if( (tp = dlerror()) != NULL) {
					mtc_err("%s", tp);
					//continue;
				} else {
					ret = (*data_handler)(hdf_get_obj(child, PRE_CFG_DATASET), dbh);
					if (ret != RET_RBTOP_OK) {
						mtc_err("%s return %d", dataer, ret);
					}
				}
			}
				
			err = cs_render(cs, &str, mcs_strcb);
			JUMP_NOK(err, wnext);
				
			snprintf(fname, sizeof(fname), PATH_DOC"%s",
					 hdf_get_value(child, PRE_CFG_OUTPUT, "null.html"));
			mutil_makesure_dir(fname);
			if(!mcs_str2file(str, fname)) {
				mtc_err("write result to %s failure", fname);
			}
#ifdef DEBUG_HDF
			snprintf(fname, sizeof(fname), "%s/hdf.%s",
					 TC_ROOT, hdf_obj_name(child));
			hdf_write_file(child, fname);
#endif
		}

	wnext:
		if (cs != NULL && outhash == NULL)
			cs_destroy(&cs);
		string_clear(&str);
		child = hdf_obj_next(child);
	}
		
	if (node != NULL) hdf_destroy(&node);

	return RET_RBTOP_OK;
}

int ltpl_parse_dir(char *dir, HASH *outhash)
{
	struct dirent **eps = NULL;
	int n;

	if (dir == NULL) {
		mtc_err("can't read null directory");
		return RET_RBTOP_INPUTE;
	}

	HASH *dbh;
	void *lib = dlopen(NULL, RTLD_NOW|RTLD_GLOBAL);
	if (lib == NULL) {
		mtc_err("possible? %s", dlerror());
		return RET_RBTOP_ERROR;
	}
	if (ldb_init(&dbh) != RET_RBTOP_OK) {
		mtc_err("init db error");
		return RET_RBTOP_INITE;
	}
	
	n = scandir(dir, &eps, tpl_config, alphasort);
	for (int i = 0; i < n; i++) {
		mtc_dbg("parse file %s", eps[i]->d_name);
		ltpl_parse_file(dbh, lib, dir, eps[i]->d_name, outhash);
		free(eps[i]);
	}

	ldb_destroy(dbh);
	dlclose(lib);
	
	if (n > 0) {
		free(eps);
		return RET_RBTOP_OK;
	} else {
		mtc_warn("not .hdf file found in %s", dir);
		return RET_RBTOP_NEXIST;
	}
}

int ltpl_init(HASH **tplh)
{
	HASH *ltplh = NULL;
	NEOERR *err;
	int ret;

	err = hash_init(&ltplh, hash_str_hash, hash_str_comp);
	RETURN_V_NOK(err, RET_RBTOP_INITE);

	ret = ltpl_parse_dir(PATH_TPL, ltplh);
	if (ret != RET_RBTOP_OK) {
		mtc_err("parse %s failure %d", PATH_TPL, ret);
		*tplh = NULL;
		return ret;
	}
	
	*tplh = ltplh;
	return RET_RBTOP_OK;
}

void ltpl_destroy(HASH *tplh)
{
	char *key = NULL;
	
	char *buf = (char*)hash_next(tplh, (void**)&key);

	while (buf != NULL) {
		free(buf);
		buf = hash_next(tplh, (void**)&key);
	}

	hash_destroy(&tplh);
}

int ltpl_render(CGI *cgi, HASH *tplh, session_t *ses)
{
	CSPARSE *cs;
	HDF *dhdf;
	STRING str;
	NEOERR *err;

	char *render = NULL, tok[64];

	render = ses->render;
	cs = (CSPARSE*)hash_lookup(tplh, render);
	if (cs == NULL) {
		mtc_err("render %s not found", render);
		return RET_RBTOP_NEXIST;
	}

	snprintf(tok, sizeof(tok), "%s_hdf", render);
	dhdf = (HDF*)hash_lookup(tplh, tok);

	hdf_copy(cgi->hdf, NULL, dhdf);
	ltpl_prepare_rend(cgi->hdf, "layout.html");
	if (ses->tm_cache_browser > 0) {
		hdf_set_valuef(cgi->hdf, "cgiout.other.cache=Cache-Control: max-age=%lu",
					   ses->tm_cache_browser);
	}
	cs->hdf = cgi->hdf;

	string_init(&str);
	err = cs_render(cs, &str, mcs_strcb);
	RETURN_V_NOK(err, RET_RBTOP_ERROR);

	err = cgi_output(cgi, &str);
	RETURN_V_NOK(err, RET_RBTOP_ERROR);

	cs->hdf = NULL;
	string_clear(&str);

	return RET_RBTOP_OK;
}
