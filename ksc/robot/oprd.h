#ifndef __PRD_H__
#define __PRD_H__

#include "mheads.h"

__BEGIN_DECLS

int prd_get(HDF *hdf, mdb_conn *conn);
int prd_add(HDF *hdf, mdb_conn *conn);
int prd_del(HDF *hdf, mdb_conn *conn);

__END_DECLS
#endif	/* __PRD_H__ */
