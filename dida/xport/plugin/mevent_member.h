#ifndef __MEVENT_MEMBER_H__
#define __MEVENT_MEMBER_H__

#define PREFIX_MEMBER   "Member"
#define PREFIX_MEMBER_PRIV   "MemberPriv"
#define PREFIX_CAR      "Car"
#define PREFIX_SPD      "Spider"

#define MEMBER_CC_SEC   60
#define CAR_CC_SEC      60
#define SPD_CC_SEC      60000

#define _COL_MEMBER "mid, mname, male, verify, credit, cityid, " \
    " to_char(intime, 'YYYY-MM-DD') as intime"
#define _COL_CAR "mid, size, model, mdate"
#define _COL_MEMBER_PRIV "mid, mname, phone, contact"

enum {
    REQ_CMD_MEMBER_GET = 1001,
    REQ_CMD_MEMBER_PRIV_GET,
    REQ_CMD_MEMBER_ADD,
    REQ_CMD_MEMBER_UP,
    REQ_CMD_CAR_GET,
    REQ_CMD_CAR_ADD,
    REQ_CMD_SPD_PEEL
};

enum {
    REP_ERR_MEMBERED = 31,
    REP_ERR_CARED,
    REP_ERR_MEMBER_NEXIST
};

#endif    /* __MEVENT_MEMBER_H__ */
