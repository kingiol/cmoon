#ifndef __OMEMBER_H__
#define __OMEMBER_H__
#include "mheads.h"

__BEGIN_DECLS

#define MEMBER_CHECK_LOGIN()                                    \
    do {                                                        \
        err = member_check_login_data_get(cgi, dbh, evth, ses); \
        if (err != STATUS_OK) return nerr_pass(err);            \
        HDF_GET_STR_IDENT(cgi->hdf, PRE_COOKIE".mname", mname); \
    } while (0)

/* TODO */
#if 0
#define LEGAL_CHECK_NICK(mnick)                                     \
    do {                                                            \
        if (!reg_search("^([a-zA-Z0-9_\.\-])+$", mnick)) {          \
            return nerr_raise(LERR_USERINPUT, "%s illegal", mnick); \
        }                                                           \
    } while(0)

#define LEGAL_CHECK_MNAME(mname)                                        \
    do {                                                                \
        if (!reg_search("^([a-zA-Z0-9_\.\-])+\@(([a-zA-Z0-9\-])+\.)+([a-zA-Z0-9]{2,4})+$", mname)) { \
            return nerr_raise(LERR_USERINPUT, "%s illegal", mname);     \
        }                                                               \
    } while(0)
#else
#define LEGAL_CHECK_NICK(mnick)
#define LEGAL_CHECK_NAME(mname)
#endif

NEOERR* member_info_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
NEOERR* member_pic_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);

NEOERR* member_edit_data_mod(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);

NEOERR* member_exist_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
NEOERR* member_new_data_add(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
NEOERR* member_login_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
NEOERR* member_logout_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
NEOERR* member_check_login_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);
NEOERR* member_reset_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses);

__END_DECLS
#endif /* __OMEMBER_H__ */
