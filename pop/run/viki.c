/*
 * viki 主脑
 */

#include "mheads.h"
#include "lheads.h"

HDF *g_cfg = NULL;

int main(int argc, char **argv, char **envp)
{
	CGI *cgi;
	NEOERR *err;
	int ret;

	HASH *dbh, *tplh, *evth;
	session_t *session = NULL;
	char *jsoncb;

	int (*data_handler)(CGI *cgi, HASH *dbh, HASH *evth, session_t *session);
	void *lib;

	//sleep(20);
	mconfig_parse_file(SITE_CONFIG, &g_cfg);
	mtc_init(TC_ROOT"viki");

	ret = ltpl_init(&tplh);
	if (ret != RET_RBTOP_OK) {
		mtc_err("init templates error");
		mutil_redirect("初始化模板失败", TGT_SELF, URL_CLOSE, true);
		return ret;
	}

	ret = ldb_init(&dbh);
	if (ret != RET_RBTOP_OK) {
		mtc_err("init db error");
		mutil_redirect("初始化数据库失败", TGT_SELF, URL_CLOSE, true);
		return ret;
	}

	ret = levt_init(&evth);
	if (ret != RET_RBTOP_OK) {
		mtc_err("init mevent error");
		mutil_redirect("初始化事件后台失败", TGT_SELF, URL_CLOSE, true);
		return ret;
	}

	lib = dlopen(NULL, RTLD_NOW|RTLD_GLOBAL);
	if (lib == NULL) {
		mtc_err("possible? %s", dlerror());
		mutil_redirect("初始化库函数失败", TGT_SELF, URL_CLOSE, true);
		return 1;
	}
	
#ifndef DROP_FCGI
	cgiwrap_init_emu(NULL, &read_cb, &printf_cb, &write_cb, NULL, NULL, NULL);
	while (FCGI_Accept() >= 0) {
#endif
		cgiwrap_init_std(argc, argv, environ);
		err = cgi_init(&cgi, NULL);
		JUMP_NOK_CGI(err, response);
		err = cgi_parse(cgi);
		JUMP_NOK_CGI(err, response);

#ifdef NCGI_MODE
		hdf_set_value(cgi->hdf, PRE_REQ_URI_RW, "/okol");
		hdf_set_value(cgi->hdf, PRE_COOKIE".uin", "1001");
		hdf_set_value(cgi->hdf, PRE_COOKIE".uname", "bigml");
		hdf_set_value(cgi->hdf, PRE_COOKIE".musn", "8Y]u0|v=*MS]U3J");
		hdf_set_value(cgi->hdf, PRE_QUERY".ip", "222.247.56.14");
		hdf_set_value(cgi->hdf, PRE_QUERY".to", "cj_BXTSJ");
		hdf_set_value(cgi->hdf, PRE_QUERY".from", "kol");
		hdf_set_value(cgi->hdf, PRE_QUERY".JsonCallback", "Ape.transport.read");
#endif
		
		ret = session_init(cgi, dbh, &session);
		if (ret != RET_RBTOP_OK) {
			mtc_err("init session failure");
			goto response;
		}

		if (mutil_client_attack(cgi->hdf, "viki",
								LMT_CLI_ATTACK, PERIOD_CLI_ATTACK)) {
			goto response;
		}
		
		data_handler = lutil_get_data_handler(lib, cgi, session);
		if (data_handler == NULL) {
			mtc_err("get handler failure");
			ret = RET_RBTOP_NEXIST;
			goto response;
		}

		ret = (*data_handler)(cgi, dbh, evth, session);
		
	response:
		if (cgi != NULL && cgi->hdf != NULL) {
			ldb_opfinish_json(ret, cgi->hdf, NULL, 0);
			
			switch (session->reqtype) {
			case CGI_REQ_HTML:
				if (CGI_REQ_METHOD(cgi) != CGI_REQ_GET) {
					goto resp_ajax;
				}
				ret = ltpl_render(cgi, tplh, session);
				if (ret != RET_RBTOP_OK) {
					if (ret == RET_RBTOP_NEXIST)
						cgi_redirect(cgi, "/404.html");
					else
						cgi_redirect(cgi, "/503.html");
				}
				break;
			case CGI_REQ_AJAX:
			resp_ajax:
				jsoncb = hdf_get_value(cgi->hdf, PRE_REQ_AJAX_FN, NULL);
				if (jsoncb != NULL) {
					//hdf_remove_tree(cgi->hdf, PRE_SUCCESS);
					mjson_execute_hdf(cgi->hdf, jsoncb, session->tm_cache_browser);
				} else {
					mjson_output_hdf(cgi->hdf, session->tm_cache_browser);
				}
				break;
			default:
				cgi_redirect(cgi, "/503.html");
				break;
			}
			
#ifdef DEBUG_HDF
			hdf_write_file(cgi->hdf, TC_ROOT"hdf.viki");
#endif
			
			cgi_destroy(&cgi);
			session_destroy(&session);
		}
#ifndef DROP_FCGI
	}
#endif

	levt_destroy(evth);
	ldb_destroy(dbh);
	ltpl_destroy(tplh);
	mconfig_cleanup(&g_cfg);
	return 0;
}
