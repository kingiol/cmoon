#ifndef __OPLAN_H__
#define __OPLAN_H__
#include "mheads.h"

__BEGIN_DECLS

NEOERR* plan_match_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);

__END_DECLS
#endif /* __OPLAN_H__ */
