#ifndef __LMISC_H__
#define __LMISC_H__
#include "mheads.h"

__BEGIN_DECLS

#define LEGAL_CK_ANAME(aname)									\
	do {														\
		if (!reg_search(aname, "^([a-zA-Z0-9_\.\-\+])+$")) {	\
			return RET_RBTOP_INPUTE;							\
		}														\
	} while(0)

#define LEGAL_CK_EMAIL(email)											\
	do {																\
		if (!reg_search(aname, "^([a-zA-Z0-9_\.\-\+])+\@(([a-zA-Z0-9\-])+\.)+([a-zA-Z0-9]{2,4})+$")) { \
			return RET_RBTOP_INPUTE;									\
		}																\
	} while(0)

__END_DECLS
#endif	/* __LMISC_H__ */
