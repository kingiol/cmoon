#ifndef __OEMAIL_H__
#define __OEMAIL_H__
#include "mheads.h"

__BEGIN_DECLS

NEOERR* email_multi_add(HDF *datanode, HASH *evth, char *emailtype);
NEOERR* email_add(HDF *datanode, HASH *evth, char *emailtype, char *mname);

__END_DECLS
#endif /* __OEMAIL_H__ */
