#ifndef __MEVENT_MEMBER_H__
#define __MEVENT_MEMBER_H__

#define PREFIX_MEMBER   "Member"
#define PREFIX_CAR      "Car"

#define MEMBER_CC_SEC   60
#define CAR_CC_SEC      60

enum {
    REQ_CMD_MEMBER_GET = 1001,
    REQ_CMD_MEMBER_ADD,
    REQ_CMD_CAR_GET,
    REQ_CMD_CAR_ADD
};

enum {
    REP_ERR_MEMBERED = 31,
    REP_ERR_CARED
};

#endif    /* __MEVENT_MEMBER_H__ */
