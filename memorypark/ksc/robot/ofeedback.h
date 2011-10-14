#ifndef __FEEDBACK_H__
#define __FEEDBACK_H__

#include "mheads.h"

__BEGIN_DECLS

int feedback_get(HDF *hdf, mdb_conn *conn);
int feedback_add(HDF *hdf, mdb_conn *conn);
int feedback_del(HDF *hdf, mdb_conn *conn);

__END_DECLS
#endif    /* __FEEDBACK_H__ */
