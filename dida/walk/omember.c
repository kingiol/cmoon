#include "mheads.h"
#include "lheads.h"
#include "omember.h"

static void member_after_login(CGI *cgi, HASH *evth, char *mname, char *mnick)
{
    char tm[LEN_TM_GMT], *p, mmsn[LEN_CK];
    mevent_t *evt = (mevent_t*)hash_lookup(evth, "member");
    if (!evt) return;

    memset(mmsn, 0x0, sizeof(mmsn));
    mstr_rand_string(mmsn, sizeof(mmsn));

    /*
     * set cookie 
     */
    cgi_cookie_set(cgi, "mname", mname, NULL, SITE_DOMAIN, NULL, 1, 0);
    cgi_cookie_set(cgi, "mnick", mnick, NULL, SITE_DOMAIN, NULL, 1, 0);
    neos_url_escape(mnick, &p, NULL);
    cgi_cookie_set(cgi, "mnick_esc", p, NULL, SITE_DOMAIN, NULL, 1, 0);
    free(p);

    //cgi_url_escape(mmsn, &p);
    mutil_getdatetime_gmt(tm, sizeof(tm), "%A, %d-%b-%Y %T GMT", 60*60*3);
    cgi_cookie_set(cgi, "mmsn", mmsn, NULL, SITE_DOMAIN, tm, 1, 0);

    hdf_set_value(evt->hdfsnd, "mname", mname);
    hdf_set_value(evt->hdfsnd, "mmsn", mmsn);
    MEVENT_TRIGGER_NRET(evt, mname, REQ_CMD_MEMBER_UP, FLAGS_NONE);

    hdf_set_copy(cgi->hdf, PRE_OUTPUT".mnick", PRE_QUERY".mnick");
    hdf_set_copy(cgi->hdf, PRE_OUTPUT".mname", PRE_QUERY".mname");
}

NEOERR* member_info_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "member");
    
    MCS_NOT_NULLB(cgi->hdf, evt);

    hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));

    MEVENT_TRIGGER(evt, NULL, REQ_CMD_MEMBER_GET, FLAGS_SYNC);

    hdf_copy(cgi->hdf, PRE_OUTPUT, evt->hdfrcv);

    return STATUS_OK;
}

NEOERR* member_pic_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "member");
    HDF *node;
    char *s = NULL, *defs = NULL;
    NEOERR *err;
    
    MCS_NOT_NULLB(cgi->hdf, evt);

    HDF_FETCH_STR(cgi->hdf, PRE_QUERY".defs", defs);

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
        mstr_html_unescape(evt->hdfrcv, s);
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

    if (!s || !*s) {
        if (!defs) s = SITE_DOMAIN;
        else if (!strcmp(defs, "segv")) return STATUS_OK;
        else s = defs;
    }
    
    err = mimg_create_from_string(s,
                                  hdf_get_value(g_cfg,
                                                "Config.font.member.path",
                                                "/usr/share/ttf/Times.ttf"),
                                  atof(hdf_get_value(g_cfg,
                                                     "Config.font.member.size",
                                                     "14.")),
                                  &ses->data);
    if (err != STATUS_OK) return nerr_pass(err);
    
    return STATUS_OK;
}

NEOERR* member_edit_data_mod(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = (mevent_t*)hash_lookup(evth, "member");
    char *mname;
    NEOERR *err;

    MEMBER_CHECK_LOGIN();
    
    /*
     * prepare data 
     */
    hdf_set_value(evt->hdfsnd, "mname", mname);
    hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));
    
    /*
     * trigger
     */
    MEVENT_TRIGGER(evt, mname, REQ_CMD_MEMBER_UP, FLAGS_NONE);
    
    return STATUS_OK;
}

NEOERR* member_exist_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "member");
    char *mname;
    
    MCS_NOT_NULLB(cgi->hdf, evt);

    HDF_GET_STR(cgi->hdf, PRE_QUERY".mname", mname);

    hdf_set_value(evt->hdfsnd, "mname", mname);

    mevent_trigger(evt, mname, REQ_CMD_MEMBER_GET, FLAGS_SYNC);

    if (PROCESS_OK(evt->errcode)) {
        hdf_set_value(cgi->hdf, PRE_OUTPUT".exist", "1");
        hdf_set_value(cgi->hdf, PRE_OUTPUT".msg", "用户已被注册");
    } else if (evt->errcode == LERR_MEMBER_NEXIST) {
        hdf_set_value(cgi->hdf, PRE_OUTPUT".exist", "0");
        hdf_set_value(cgi->hdf, PRE_OUTPUT".msg", "用户还未注册");
    } else {
        return nerr_raise(evt->errcode, "get member %s info failure %d",
                          evt->ename, evt->errcode);
    }

    return STATUS_OK;
}

NEOERR* member_new_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "member");
    char *mnick, *mname;
    
    MCS_NOT_NULLB(cgi->hdf, evt);

    HDF_GET_STR(cgi->hdf, PRE_QUERY".mnick", mnick);
    HDF_GET_STR(cgi->hdf, PRE_QUERY".mname", mname);

    LEGAL_CHECK_NICK(mnick);
    LEGAL_CHECK_NAME(mname);

    hdf_copy(evt->hdfsnd, NULL, hdf_get_obj(cgi->hdf, PRE_QUERY));

    MEVENT_TRIGGER(evt, mname, REQ_CMD_MEMBER_ADD, FLAGS_SYNC);

    member_after_login(cgi, evth, mname, mnick);

    return STATUS_OK;
}

NEOERR* member_login_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "member");
    char *mname, *msn;
    
    MCS_NOT_NULLB(cgi->hdf, evt);

    HDF_GET_STR(cgi->hdf, PRE_QUERY".mname", mname); 
    HDF_GET_STR(cgi->hdf, PRE_QUERY".msn", msn);

    hdf_set_value(evt->hdfsnd, "mname", mname);

    MEVENT_TRIGGER(evt, mname, REQ_CMD_MEMBER_GET, FLAGS_SYNC);

    char *msndb = hdf_get_value(evt->hdfrcv, "msn", NULL);
    char *mnick = hdf_get_value(evt->hdfrcv, "mnick", "嘀嗒");
    if (msndb) {
        if (!strcmp(msndb, msn)) {
            member_after_login(cgi, evth, mname, mnick);
            return STATUS_OK;
        } else 
            return nerr_raise(LERR_LOGINPSW, "password wrong %s %s", msndb, msn);
    }

    return nerr_raise(LERR_MEMBER_NEXIST, "member %s not exist", mname);
}

NEOERR* member_logout_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "member");
    char *mname;
    NEOERR *err;
    
    MCS_NOT_NULLB(cgi->hdf, evt);

    MEMBER_CHECK_LOGIN();

    HDF_GET_STR_IDENT(cgi->hdf, PRE_COOKIE".mname", mname);
    
    hdf_set_value(evt->hdfsnd, "mname", mname);
    hdf_set_value(evt->hdfsnd, "mmsn", "0");

    MEVENT_TRIGGER(evt, mname, REQ_CMD_MEMBER_UP, FLAGS_NONE);

    return STATUS_OK;
}

NEOERR* member_check_login_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = hash_lookup(evth, "member");
    char *mname, *mmsn;
    
    MCS_NOT_NULLB(cgi->hdf, evt);

    HDF_GET_STR_IDENT(cgi->hdf, PRE_COOKIE".mname", mname);
    HDF_GET_STR_IDENT(cgi->hdf, PRE_COOKIE".mmsn", mmsn);

    hdf_set_value(evt->hdfsnd, "mname", mname);

    MEVENT_TRIGGER(evt, mname, REQ_CMD_MEMBER_GET, FLAGS_SYNC);

    char *mmsndb = hdf_get_value(evt->hdfrcv, "mmsn", NULL);
    if (mmsndb) {
        if (!strcmp(mmsndb, mmsn)) {
            return STATUS_OK;
        }
    }
    mtc_warn("%s<====>%s", mmsndb, mmsn);

    return nerr_raise(LERR_NOTLOGIN, "%s not login", mname);
}

NEOERR* member_reset_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt;
    char *mnick, *mname, *sub, *content;
    char rlink[LEN_CK];
    
    HDF_GET_STR(cgi->hdf, PRE_QUERY".mname", mname);
    mstr_rand_string(rlink, sizeof(rlink));
    
    evt = (mevent_t*)hash_lookup(evth, "member");
    MCS_NOT_NULLB(cgi->hdf, evt);

    hdf_set_value(evt->hdfsnd, "mname", mname);
    hdf_set_value(evt->hdfsnd, "rlink", rlink);

    MEVENT_TRIGGER(evt, mname, REQ_CMD_MEMBER_SETRLINK, FLAGS_SYNC);

    mnick = hdf_get_value(evt->hdfrcv, "mnick", NULL);

    /*
     * input check
     */
    evt = (mevent_t*)hash_lookup(evth, "aux");
    MCS_NOT_NULLA(evt);

    sub = hdf_get_value(g_cfg, "Email.MemberReset.subject", NULL);
    content = hdf_get_value(g_cfg, "Email.MemberReset.content", NULL);
    MCS_NOT_NULLB(sub, content);

    /*
     * prepare data 
     */
    hdf_set_value(evt->hdfsnd, "go", "1");
    hdf_set_value(evt->hdfsnd, "opt",
                  hdf_get_value(g_cfg, "Email.MemberReset.opt", NULL));
    hdf_set_value(evt->hdfsnd, "sub", sub);
    hdf_set_value(evt->hdfsnd, "to", mname);

    char *p = NULL;
    neos_url_escape(mname, &p, NULL);
    content = mstr_repstr(3, content, "$mnick$", mnick, "$mname_esc$", p, "$rlink$", rlink);
    hdf_set_value(evt->hdfsnd, "content", content);
    SAFE_FREE(content);
    SAFE_FREE(p);
    
    /*
     * trigger
     */
    MEVENT_TRIGGER(evt, mname, REQ_CMD_MAIL_ADD, FLAGS_NONE);

    return STATUS_OK;
}

NEOERR* member_pass_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
    mevent_t *evt = (mevent_t*)hash_lookup(evth, "member");
    char *mnick, *mname, *rlink;
    NEOERR *err;

    MCS_NOT_NULLB(cgi->hdf, evt);

    HDF_FETCH_STR(cgi->hdf, PRE_QUERY".mname", mname);
    HDF_FETCH_STR(cgi->hdf, PRE_QUERY".rlink", rlink);

    if (mname && rlink) {
        hdf_set_value(evt->hdfsnd, "mname", mname);
        MEVENT_TRIGGER(evt, mname, REQ_CMD_MEMBER_GETRLINK, FLAGS_SYNC);

        mnick = hdf_get_value(evt->hdfrcv, "mnick", NULL);

        char *s = hdf_get_value(evt->hdfrcv, "rlink", NULL);
        if (!s || strcmp(s, rlink))
            return nerr_raise(LERR_USERINPUT, "验证码错误");

        member_after_login(cgi, evth, mname, mnick);
        
        hdf_set_value(cgi->hdf, PRE_OUTPUT".mname", mname);

        return STATUS_OK;
    }

    MEMBER_CHECK_LOGIN();

    hdf_set_value(cgi->hdf, PRE_OUTPUT".mname", mname);
    
    return STATUS_OK;
}
