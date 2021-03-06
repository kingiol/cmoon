#ifndef __LNUM_H__
#define __LNUM_H__

#include "mheads.h"

__BEGIN_DECLS

enum user_status
{
    USER_FRESH = 0,
    USER_RLSED,
    USER_CFMED
};

enum group_stat {
    GROUP_STAT_APPLY = 0,
    GROUP_STAT_REJECT,
    GROUP_STAT_INVITE,
    GROUP_STAT_REFUSE,
    GROUP_STAT_OK,
    GROUP_STAT_ALL = 255
};

/*
 * JUNIOR: add/del group member
 * SENIOR: MOD sys-file mode; ADD sub sys-file to myself
 * ADMIN:  DEL sys-file; MANAGE group's >JOIN member
 */
enum group_mode {
    GROUP_MODE_JOIN = 0,
    GROUP_MODE_JUNIOR,
    GROUP_MODE_SENIOR,
    GROUP_MODE_ADM,
    GROUP_MODE_OWN,
    GROUP_MODE_ROOT = 255
};

enum limit_type {
    LMT_TYPE_MEMBER = 1,
    LMT_TYPE_GJOIN,
    LMT_TYPE_JUNIOR,
    LMT_TYPE_SENIOR,
    LMT_TYPE_GADM,
    LMT_TYPE_GOWN,
    LMT_TYPE_ROOT = 255
};

enum uiplug_type {
    UIPLUG_TYPE_MOLE = 0,
    UIPLUG_TYPE_ATOM
};

#define MIN_USER_NUM    998

#define LMT_CLI_REGIST    400
#define LMT_CLI_LOGIN    400

#define PERIOD_CLI_REGIST    ONE_HOUR
#define PERIOD_CLI_LOGIN    ONE_HOUR

#define LMT_CLI_ATTACK        100
#define PERIOD_CLI_ATTACK    ONE_MINUTE

#define DIV_USER_TB        100

#define MAX_URI_ITEM    100
#define MAX_GROUP_AUSER    100

#define RET_RBTOP_LOGINUSR    2001
#define RET_RBTOP_LOGINPSW    2002
#define RET_RBTOP_RELEASED    2003

#define RET_RBTOP_GETLISTE    2010
#define RET_RBTOP_SPLITSTRE    2011

#define RET_RBTOP_MMCERR    2020

#define RET_RBTOP_NOTLOGIN    2030
#define RET_RBTOP_LIMITE    2031

#define RET_RBTOP_NEXIST    2040
#define RET_RBTOP_EXISTE    2041

#define RET_RBTOP_IMGPROE    2050

__END_DECLS
#endif    /* __LNUM_H__ */
