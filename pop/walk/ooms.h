#ifndef __OOMS_H__
#define __OOMS_H__
#include "mheads.h"

__BEGIN_DECLS

int oms_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
int oms_camer_data_del(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);

int oms_edit_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
int oms_edit_data_mod(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);

int oms_users_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
int oms_users_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
int oms_users_data_del(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);

int oms_secy_data_mod(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);

__END_DECLS
#endif /* __OOMS_H__ */
