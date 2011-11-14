#ifndef __MEVENT_PLAN_H__
#define __MEVENT_PLAN_H__

#define PREFIX_PLAN     "Plan"

#define PLAN_CC_SEC     60

#define _COL_PLAN "id, mid, dad, nick, pstatu, repeat, seat, fee, attach, " \
    " scityid, ecityid, saddr, eaddr, marks, rect, route, km, "         \
    " sdate, edate, stime, etime, estmin, "                             \
    " to_char(intime, 'YYYY-MM-DD') as intime, "                        \
    " to_char(uptime, 'YYYY-MM-DD') as uptime "

enum {
    REQ_CMD_PLAN_GET = 1001,
    REQ_CMD_PLAN_ADD,
    REQ_CMD_PLAN_UP
};

enum {
    PLAN_ST_FRESH = 0,
    PLAN_ST_SPD_OK,
    PLAN_ST_PAUSE,
    PLAN_ST_DELETE,
    PLAN_ST_SPD_FRESH = 10
};

enum {
    REP_ERR_PLAN_NEXIST = 33
};

#endif    /* __MEVENT_PLAN_H__ */
