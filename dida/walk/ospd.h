#ifndef __OSPD_H__
#define __OSPD_H__
#include "mheads.h"

__BEGIN_DECLS

NEOERR* spd_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
NEOERR* spd_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);

__END_DECLS
#endif /* __OSPD_H__ */
