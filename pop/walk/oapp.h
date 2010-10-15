#ifndef __OAPP_H__
#define __OAPP_H__
#include "mheads.h"

__BEGIN_DECLS

#define APP_CHECK_LOGIN()												\
	do {																\
		if (app_check_login_data_get(cgi, dbh, evth, ses) != RET_RBTOP_OK) { \
			mtc_warn("doesen't login");									\
			return RET_RBTOP_NOTLOGIN;									\
		}																\
		HDF_GET_STR_IDENT(cgi->hdf, PRE_COOKIE".aname", aname);			\
		LEGAL_CK_ANAME(aname);											\
	} while (0)

#define APP_CHECK_LOGIN_ADMIN(uname)									\
	do {																\
		if (app_check_login_data_get(cgi, dbh, evth, ses) != RET_RBTOP_OK) { \
			mtc_warn("doesen't login");									\
			return RET_RBTOP_NOTLOGIN;									\
		}																\
		if (hdf_get_int_value(evt->hdfrcv, "pid", -1) != 0) {			\
			mtc_warn("%s not super admin", aname);						\
			return RET_RBTOP_LIMITE;									\
		}																\
		HDF_GET_STR_IDENT(cgi->hdf, PRE_COOKIE".aname", aname);			\
		LEGAL_CK_ANAME(aname);											\
		/* we can also get uname's info instead of get aname's ousers */ \
		hdf_set_value(evt->hdfsnd, "pname", aname);						\
		MEVENT_TRIGGER(RET_RBTOP_EVTE, evt, aname, REQ_CMD_APP_O_USERS, FLAGS_SYNC); \
		if (!hdf_get_valuef(evt->hdfrcv, "users.%s.aname", uname)) {	\
			mtc_warn("%s don't own %s", aname, uname);					\
			return RET_RBTOP_LIMITE;									\
		}																\
	} while (0)

int app_exist_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
int app_new_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
/* sb want to login */
int app_login_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
int app_logout_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
/* sb want to do sth serious, need be verify */
int app_check_login_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);


__END_DECLS
#endif /* __OAPP_H__ */
