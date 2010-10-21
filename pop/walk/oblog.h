#ifndef __OBLOG_H__
#define __OBLOG_H__
#include "mheads.h"

__BEGIN_DECLS

#define BLOG_SUBDIR_NUM		32
#define BLOG_NUM_PERPAGE	10

enum {
	BLOG_ST_NORMAL = 0,
	BLOG_ST_DEL
} blog_state;

int blog_index_static_get(HDF *hdf, HASH *dbh);
int blog_static_get(HDF *hdf, HASH *dbh);

int blog_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
int blog_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
int blog_data_mod(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
int blog_data_del(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);

__END_DECLS
#endif /* __OBLOG_H__ */
