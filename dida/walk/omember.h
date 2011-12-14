#ifndef __OMEMBER_H__
#define __OMEMBER_H__
#include "mheads.h"

__BEGIN_DECLS

NEOERR* member_info_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
NEOERR* member_phone_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);

__END_DECLS
#endif /* __OMEMBER_H__ */
