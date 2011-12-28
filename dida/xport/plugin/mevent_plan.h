#ifndef __MEVENT_PLAN_H__
#define __MEVENT_PLAN_H__

#define PREFIX_PLAN     "Plan"
#define PREFIX_SPD      "Spd"

#define PLAN_CC_SEC     60
#define SPD_CC_SEC      60000

#define _COL_PLAN "id, mid, ori, ourl, dad, nick, statu, repeat, seat, " \
    " fee, attach, scityid, ecityid, saddr, eaddr, marks, rect, route, " \
    " km, sdate, edate, stime, etime, estmin, "                         \
    " to_char(intime, 'YYYY-MM-DD') as intime, "                        \
    " to_char(uptime, 'YYYY-MM-DD') as uptime "

enum {
    REQ_CMD_PLAN_GET = 1001,
    REQ_CMD_PLAN_GET_BY_GEO,
    REQ_CMD_PLAN_ADD,
    REQ_CMD_PLAN_UP,
    REQ_CMD_SPD_PEEL
};

enum {
    PLAN_ST_FRESH = 0,
    PLAN_ST_SPD_OK,
    PLAN_ST_PAUSE,
    PLAN_ST_DELETE,
    PLAN_ST_SPD_FRESH = 10
};

enum {
    PLAN_DAD_MAN = 0,
    PLAN_DAD_CAR
};

enum {
    PLAN_RPT_NONE = 0,
    PLAN_RPT_DAY,
    PLAN_RPT_WEEK
};

enum {
    REP_ERR_PLAN_NEXIST = 34,
    REP_ERR_PLAN_NMATCH,
    REP_ERR_PLANED
};

#endif    /* __MEVENT_PLAN_H__ */
