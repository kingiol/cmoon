#include "mheads.h"
#include "lheads.h"
#include "oemail.h"

NEOERR* email_multi_add(HDF *datanode, HASH *evth, char *emailtype)
{
    mevent_t *evt;
    char *mname;
    HDF *child;
    NEOERR *err;
    
    MCS_NOT_NULLB(datanode, evth);

    evt = hash_lookup(evth, "aux");
    MCS_NOT_NULLA(evt);

    child = hdf_obj_child(datanode);
    while (child) {
        mname = hdf_get_value(child, "mname", NULL);
        if (mname) {
            hdf_set_valuef(evt->hdfsnd, "mto.%s=%s", mname, mname);
        }

        child = hdf_obj_next(child);
    }

    if (!hdf_get_obj(evt->hdfsnd, "mto"))
        return nerr_raise(NERR_ASSERT, "请设置收信用户名");

    err = mcs_hdf_copy_rep(evt->hdfsnd, NULL,
                           mcs_hdf_getf(g_cfg, "Email.%s", emailtype),
                           hdf_get_obj(datanode, PRE_DATASET));
	if (err != STATUS_OK) return nerr_pass(err);
    
    MEVENT_TRIGGER(evt, NULL, REQ_CMD_AUX_EMAIL_ADD, FLAGS_NONE);

    return STATUS_OK;
}

NEOERR* email_add(HDF *datanode, HASH *evth, char *emailtype, char *mname)
{
    mevent_t *evt;
    NEOERR *err;
    
    MCS_NOT_NULLC(datanode, evth, mname);

    evt = hash_lookup(evth, "aux");
    MCS_NOT_NULLA(evt);

    err = mcs_hdf_copy_rep(evt->hdfsnd, NULL,
                           mcs_hdf_getf(g_cfg, "Email.%s", emailtype),
                           hdf_get_obj(datanode, PRE_DATASET));
	if (err != STATUS_OK) return nerr_pass(err);

    hdf_set_value(evt->hdfsnd, "to", mname);

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_AUX_EMAIL_ADD, FLAGS_NONE);

    return STATUS_OK;
}
