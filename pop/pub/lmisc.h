#ifndef __LMISC_H__
#define __LMISC_H__
#include "mheads.h"

__BEGIN_DECLS

#if 0
#define LEGAL_CK_ANAME(aname)								\
	do {													\
		if (!reg_search("^([a-zA-Z0-9_\.\-])+$", aname)) {	\
			mtc_err("%s illegal", aname);					\
			return RET_RBTOP_INPUTE;						\
		}													\
	} while(0)

#define LEGAL_CK_EMAIL(email)											\
	do {																\
		if (!reg_search("^([a-zA-Z0-9_\.\-])+\@(([a-zA-Z0-9\-])+\.)+([a-zA-Z0-9]{2,4})+$", email)) { \
			mtc_err("%s illegal", email);								\
			return RET_RBTOP_INPUTE;									\
		}																\
	} while(0)
#else

#define LEGAL_CK_ANAME(aname)
#define LEGAL_CK_EMAIL(email)

#endif

__END_DECLS
#endif	/* __LMISC_H__ */
