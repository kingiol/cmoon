#ifndef __LOGIN_H__
#define __LOGIN_H__

#include "mheads.h"

__BEGIN_DECLS

int login_get(CGI *cgi, mdb_conn *conn);
int login_check(HDF *hdf, mdb_conn *conn);

__END_DECLS
#endif	/* __LOGIN_H__ */
