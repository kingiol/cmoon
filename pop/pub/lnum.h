#ifndef __LNUM_H__
#define __LNUM_H__

#include "mheads.h"

__BEGIN_DECLS

enum uiplug_type {
    UIPLUG_TYPE_MOLE = 0,
    UIPLUG_TYPE_ATOM
};

#define LMT_CLI_REGIST	400
#define LMT_CLI_LOGIN	400

#define PERIOD_CLI_REGIST	ONE_HOUR
#define PERIOD_CLI_LOGIN	ONE_HOUR

#define LMT_CLI_ATTACK		100
#define PERIOD_CLI_ATTACK	ONE_MINUTE

#define MAX_URI_ITEM	100
#define MAX_GROUP_AUSER	100

#define RET_RBTOP_LOGINUSR	2001
#define RET_RBTOP_LOGINPSW	2002
#define RET_RBTOP_RELEASED	2003

#define RET_RBTOP_GETLISTE	2010
#define RET_RBTOP_SPLITSTRE	2011

#define RET_RBTOP_MMCERR	2020

#define RET_RBTOP_NOTLOGIN	2030
#define RET_RBTOP_LIMITE	2031

#define RET_RBTOP_NEXIST	2040
#define RET_RBTOP_EXISTE	2041

#define RET_RBTOP_IMGPROE	2050
#define RET_RBTOP_OPENFILE	2051

__END_DECLS
#endif	/* __LNUM_H__ */
