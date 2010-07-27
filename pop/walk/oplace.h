#ifndef __OPLACE_H__
#define __OPLACE_H__
#include "mheads.h"

__BEGIN_DECLS

#define	REDIRECT_MODE_1 0x01
#define	REDIRECT_MODE_2 0x02
#define IP_RECORD_LENGTH 7

int ip2addr_data_get(char *ip, char **c, char **a);
int place_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);

__END_DECLS
#endif /* __OPLACE_H__ */
