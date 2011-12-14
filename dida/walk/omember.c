#include "mheads.h"
#include "lheads.h"
#include "omember.h"

NEOERR* member_info_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "member");
    
    if (!cgi || !cgi->hdf || !evt) return nerr_raise(NERR_ASSERT, "paramter null");

    hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_MEMBER_GET, FLAGS_SYNC);

    hdf_copy(cgi->hdf, PRE_OUTPUT, evt->hdfrcv);

    return STATUS_OK;
}

NEOERR* member_pic_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "member");
    HDF *node;
    char *s = NULL;
    
    if (!cgi || !cgi->hdf || !evt) return nerr_raise(NERR_ASSERT, "paramter null");

    node = hdf_get_child(cgi->hdf, PRE_QUERY".type");
    if (!node) HDF_GET_STR(cgi->hdf, PRE_QUERY".type", s);
    
    hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_MEMBER_PRIV_GET, FLAGS_SYNC);

    if (s) goto getval;
    while (node) {
        s = hdf_obj_value(node);
    getval:
        /*
         * turn &amp; into & in url format
         */
        mcs_html_unescape(evt->hdfrcv, s);
        s = hdf_get_value(evt->hdfrcv, s, NULL);
        if (s && *s) {
            if (!strncmp(s, "http:", 5)) {
                hdf_set_value(cgi->hdf, PRE_OUTPUT".302", s);
                return STATUS_OK;
            }
            break;
        }
        
        node = hdf_obj_next(node);
    }

    if (!s || !*s) s = SITE_DOMAIN;

    /*
     * drawing the string
     */
    gdImagePtr im;
    int draw, back, brect[8], x, y;
    char *f, *gderr;
    double sz;

    f = hdf_get_value(g_cfg, "Config.font.member.path", "/usr/share/ttf/Times.ttf");
    sz = atof(hdf_get_value(g_cfg, "Config.font.member.size", "14."));

    gderr = gdImageStringFT(NULL, &brect[0], 0, f, sz, 0., 0, 0, s);
    if (gderr) return nerr_raise(NERR_ASSERT, "create image failure %s", gderr);
    
    x = brect[2]-brect[6] + 6;
    y = brect[3]-brect[7] + 6;
    im = gdImageCreate(x, y);

    /* background color */
    back = gdImageColorResolve(im, 252, 252, 252);
    /* foreground color */
    draw = gdImageColorResolve(im, 150, 40, 40);
    
    x = 3 - brect[6];
    y = 3 - brect[7];

    gderr = gdImageStringFT(im, &brect[0], draw, f, sz, 0.0, x, y, s);
    if (gderr) return nerr_raise(NERR_ASSERT, "rend image failure %s", gderr);

    ses->data = (void*) im;
    
    return STATUS_OK;
}
