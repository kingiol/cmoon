#ifndef __OBORE_H__
#define __OBORE_H__
#include "mheads.h"

__BEGIN_DECLS

#define _COL_MEMORY "id, statu, title, hint, content, previd, nextid, " \
    " to_char(intime, 'YYYY-MM-DD') as intime, "                        \
    " to_char(uptime, 'YYYY-MM-DD') as uptime "

enum {
    MEMORY_ST_OK = 0,
    MEMORY_ST_DEL
};

NEOERR* bore_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
NEOERR* bore_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);

__END_DECLS
#endif /* __OBORE_H__ */
