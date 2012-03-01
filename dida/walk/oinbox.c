#include "mheads.h"
#include "lheads.h"
#include "oinbox.h"

NEOERR* inbox_multi_add(HDF *datanode, HASH *evth, char *inboxtype)
{
    mevent_t *evt;
    int mid;
    HDF *child;
    NEOERR *err;
    
    MCS_NOT_NULLB(datanode, evth);

    evt = hash_lookup(evth, "aux");
    MCS_NOT_NULLA(evt);

    child = hdf_obj_child(datanode);
    while (child) {
        mid = hdf_get_int_value(child, "mid", 0);
        if (mid > 0) {
            hdf_set_valuef(evt->hdfsnd, "mmid.%d=%d", mid, mid);
        }

        child = hdf_obj_next(child);
    }

    if (!hdf_get_obj(evt->hdfsnd, "mmid"))
        return nerr_raise(NERR_ASSERT, "请设置收信用户号码");

    err = mcs_hdf_copy_rep(evt->hdfsnd, NULL,
                           mcs_hdf_getf(g_cfg, "Inbox.%s", inboxtype),
                           hdf_get_obj(datanode, PRE_DATASET));
	if (err != STATUS_OK) return nerr_pass(err);
    
    MEVENT_TRIGGER(evt, NULL, REQ_CMD_AUX_INBOX_ADD, FLAGS_NONE);

    return STATUS_OK;
}
