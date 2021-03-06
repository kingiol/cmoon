#ifndef __MEVENT_BANK_H__
#define __MEVENT_BANK_H__

#define PREFIX_BANK        "Bank"
#define PREFIX_BILL        "Bill"
#define BANK_CC_SEC        60
#define BILL_CC_SEC        60

enum {
    BANK_OP_PRECHARGE = 0,
    BANK_OP_CFMCHARGE,
    BANK_OP_ADDACCOUNT,
    BANK_OP_ROLLBACK
};

enum {
    REQ_CMD_BANK_INFO = 1001,
    REQ_CMD_BANK_ADDBILL,
    REQ_CMD_BANK_GETBILL
};

enum {
    REP_ERR_BANK_NCHARGE = 33,
    REP_ERR_BANK_NEEDUP,
    REP_ERR_BANK_NEEDWT
};

#endif    /* __MEVENT_BANK_H__ */
