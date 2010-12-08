#include "mheads.h"
#include "lheads.h"

/* LERR_XXX start from 14 */
int LERR_MEVENT = 0;
int LERR_NOTLOGIN = 0;
int LERR_LOGINPSW = 0;
int LERR_USERINPUT = 0;
int LERR_LIMIT = 0;
int LERR_NEEDVIP = 0;
int LERR_NEEDVVIP = 0;
int LERR_NEXIST = 0;
int LERR_EXIST = 0;
int LERR_IMGPROE = 0;
int LERR_ATTACK = 0;
static int lerrInited = 0;

NEOERR* lerr_init()
{
	NEOERR *err;

	if (lerrInited == 0) {
		err = nerr_init();
		if (err != STATUS_OK) return nerr_pass(err);
		err = nerr_register(&LERR_MEVENT, "后台处理失败");
		if (err != STATUS_OK) return nerr_pass(err);
		err = nerr_register(&LERR_NOTLOGIN, "请登录后操作");
		if (err != STATUS_OK) return nerr_pass(err);
		err = nerr_register(&LERR_LOGINPSW, "密码错误");
		if (err != STATUS_OK) return nerr_pass(err);
		err = nerr_register(&LERR_USERINPUT, "输入参数错误");
		if (err != STATUS_OK) return nerr_pass(err);
		err = nerr_register(&LERR_LIMIT, "用户无权限");
		if (err != STATUS_OK) return nerr_pass(err);
		err = nerr_register(&LERR_NEEDVIP, "专业版功能");
		if (err != STATUS_OK) return nerr_pass(err);
		err = nerr_register(&LERR_NEEDVVIP, "完全版功能");
		if (err != STATUS_OK) return nerr_pass(err);
		err = nerr_register(&LERR_NEXIST, "资源不存在");
		if (err != STATUS_OK) return nerr_pass(err);
		err = nerr_register(&LERR_EXIST, "资源已存在");
		if (err != STATUS_OK) return nerr_pass(err);
		err = nerr_register(&LERR_IMGPROE, "处理图片失败");
		if (err != STATUS_OK) return nerr_pass(err);
		err = nerr_register(&LERR_ATTACK, "休息会儿");
		if (err != STATUS_OK) return nerr_pass(err);

		lerrInited = 1;
	}

	return STATUS_OK;
}

void lerr_opfinish_json(NEOERR *err, HDF *hdf)
{
	if (err == STATUS_OK) {
		hdf_set_value(hdf, PRE_SUCCESS, "1");
		return;
	}
	
	hdf_remove_tree(hdf, PRE_SUCCESS);
	
	char buf[1024];
	NEOERR *neede = mcs_err_valid(err);
	if (!neede) neede = err;
	/* set PRE_ERRXXX with the most recently err */
	if (!hdf_get_obj(hdf, PRE_ERRMSG)) {
		hdf_set_value(hdf, PRE_ERRMSG, _lookup_errname(neede, buf, sizeof(buf)));
	}
	hdf_set_int_value(hdf, PRE_ERRCODE, neede->error);

	STRING str; string_init(&str);
	nerr_error_traceback(err, &str);
	mtc_err("%s", str.buf);
	hdf_set_value(hdf, PRE_ERRTRACE, str.buf);
	nerr_ignore(&err);
	string_clear(&str);
}
