#ifndef __OPOS_H__
#define __OPOS_H__
#include "mheads.h"

__BEGIN_DECLS

NEOERR* pos_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
NEOERR* pos_data_mod(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);

__END_DECLS
#endif /* __OPOS_H__ */
