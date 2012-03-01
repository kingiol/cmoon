#ifndef __OINBOX_H__
#define __OINBOX_H__
#include "mheads.h"

__BEGIN_DECLS

NEOERR* inbox_multi_add(HDF *datanode, HASH *evth, char *inboxtype);

__END_DECLS
#endif /* __OINBOX_H__ */
