#ifndef __OAPP_H__
#define __OAPP_H__
#include "mheads.h"

__BEGIN_DECLS

int app_exist_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
int app_new_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
/* sb want to login */
int app_login_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
int app_logout_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
/* sb want to do sth serious, need be verify */
int app_check_login_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);


__END_DECLS
#endif /* __OAPP_H__ */
