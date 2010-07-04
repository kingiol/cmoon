#ifndef __OBLOG_H__
#define __OBLOG_H__
#include "mheads.h"

__BEGIN_DECLS

#define BLOG_INDEX	PATH_DOC"index_blog"

int blog_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);

__END_DECLS
#endif /* __OBLOG_H__ */
