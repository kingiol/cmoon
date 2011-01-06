#ifndef __LEVT_H__
#define __LEVT_H__
#include "mheads.h"

__BEGIN_DECLS

/* TODO hdf_write_string lead mem_leak */
#define MEVENT_TRIGGER(evt, key, cmd, flags)							\
	do {																\
		if (PROCESS_NOK(mevent_trigger(evt, key, cmd, flags))) {		\
			char *zpa = NULL;											\
			switch (evt->errcode) {										\
			case REP_ERR_ALREADYREGIST:									\
				return nerr_raise(LERR_EXIST, "%s already exist", key ? key : "null"); \
			case REP_ERR_NREGIST:										\
				return nerr_raise(LERR_NEXIST, "%s don't regist", key ? key : "null"); \
			default:													\
				hdf_write_string(evt->hdfrcv, &zpa);					\
				return nerr_raise(LERR_MEVENT, "pro %s %d failure %d %s", \
								  evt->ename, cmd, evt->errcode, zpa);	\
			}															\
		}																\
	} while(0)
#define MEVENT_TRIGGER_VOID(evt, key, cmd, flags)						\
	do {																\
		if (PROCESS_NOK(mevent_trigger(evt, key, cmd, flags))) {		\
			char *zpa = NULL;											\
			hdf_write_string(evt->hdfrcv, &zpa);						\
			mtc_err("pro %s %d failure %d %s", evt->ename, cmd, evt->errcode, zpa); \
			if (zpa) free(zpa);											\
			return;														\
		}																\
	} while(0)
#define MEVENT_TRIGGER_NRET(evt, key, cmd, flags)						\
	do {																\
		if (PROCESS_NOK(mevent_trigger(evt, key, cmd, flags))) {		\
			char *zpa = NULL;											\
			hdf_write_string(evt->hdfrcv, &zpa);						\
			mtc_err("pro %s %d failure %d %s", evt->ename, cmd, evt->errcode, zpa); \
			if (zpa) free(zpa);											\
		}																\
	} while(0)

NEOERR* levt_init(HASH **evth);
void levt_destroy(HASH *evth);

__END_DECLS
#endif	/* __LEVT_H__ */
