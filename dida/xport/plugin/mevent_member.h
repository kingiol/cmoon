#ifndef __MEVENT_MEMBER_H__
#define __MEVENT_MEMBER_H__

#define PREFIX_MEMBER   "Member"
#define PREFIX_MEMBER_PRIV   "MemberPriv"
#define PREFIX_CAR      "Car"

#define MEMBER_CC_SEC   60
#define CAR_CC_SEC      60

#define _COL_MEMBER "mid, mname, ori, male, verify, credit, cityid, "   \
    " to_char(intime, 'YYYY-MM-DD') as intime"
#define _COL_MEMBER_PRIV "mid, mname, phone, contact"
#define _COL_MEMBER_ADMIN "mid, mname, male, phone, contact, "  \
    " verify, credit, cityid, "                                 \
    " to_char(intime, 'YYYY-MM-DD') as intime"
#define _COL_CAR "mid, size, model, mdate"

enum {
    REQ_CMD_MEMBER_GET = 1001,
    REQ_CMD_MEMBER_PRIV_GET,
    REQ_CMD_MEMBER_ADD,
    REQ_CMD_MEMBER_UP,
    REQ_CMD_CAR_GET,
    REQ_CMD_CAR_ADD
};

enum {
    MEMBER_ST_FRESH = 0,
    MEMBER_ST_SPD_OK,
    MEMBER_ST_PAUSE,
    MEMBER_ST_DELETE,
    MEMBER_ST_SPD_FRESH = 10
};

enum {
    REP_ERR_MEMBERED = 31,
    REP_ERR_CARED,
    REP_ERR_MEMBER_NEXIST
};

#endif    /* __MEVENT_MEMBER_H__ */
