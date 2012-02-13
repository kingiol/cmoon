#ifndef __MEVENT_AUX_H__
#define __MEVENT_AUX_H__

#define PREFIX_COMMENT     "Comment"

enum {
    CMT_ST_NORMAL = 0,
    CMD_ST_DEL
};

enum {
    CMT_TYPE_MEMORY = 0
};

enum {
    REQ_CMD_CMT_GET = 1001,
    REQ_CMD_CMT_ADD,
    REQ_CMD_MAIL_ADD = 2001
};

#endif    /* __MEVENT_AUX_H__ */
