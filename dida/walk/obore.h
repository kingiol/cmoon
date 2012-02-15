#ifndef __OBORE_H__
#define __OBORE_H__
#include "mheads.h"

__BEGIN_DECLS

NEOERR* bore_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);

NEOERR* bore_new_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
NEOERR* bore_op_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
NEOERR* bore_op_data_mod(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
NEOERR* bore_op_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
NEOERR* bore_preview_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);

__END_DECLS
#endif /* __OBORE_H__ */
