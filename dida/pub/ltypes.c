#include "mheads.h"
#include "lheads.h"

NEOERR* session_init(CGI *cgi, HASH *dbh, session_t **ses)
{
    session_t *lses;
    char tok[_POSIX_PATH_MAX];

    HDF *node = hdf_get_child(cgi->hdf, PRE_QUERY);
    while (node) {
        snprintf(tok, sizeof(tok), "%s._type_%s", PRE_QUERY, hdf_obj_name(node));
        char *type = hdf_get_value(cgi->hdf, tok, NULL);
        if (type && !strcmp(type, "object")) {
            mjson_str2hdf(node, NULL);
        }

        node = hdf_obj_next(node);
    }
    
    *ses = NULL;

    lses = calloc(1, sizeof(session_t));
    if (!lses) return nerr_raise(NERR_NOMEM, "calloc memory for session_t failure");
    
    lses->reqtype = CGI_REQ_HTML;
    hdf_get_copy(cgi->hdf, PRE_COOKIE".mname", &lses->mname, NULL);

    char *uri = hdf_get_value(cgi->hdf, PRE_REQ_URI_RW, NULL);
    if (!uri) {
        uri = "terminal";
        lses->reqtype = CGI_REQ_TERMINAL;
    }
    /* TODO uniq req uri */
    //uri = mmisc_str_uniq(uri, '/');
    mmisc_str_repchr(uri, '/', '_');
    uri = mmisc_str_strip(uri, '_');
    if (!strncmp(uri, "json_", 5)) {
        uri = uri+5;
        lses->reqtype = CGI_REQ_AJAX;
    } else if (!strncmp(uri, "image_", 6)) {
        uri = uri+6;
        lses->reqtype = CGI_REQ_IMAGE;
    }
    switch (CGI_REQ_METHOD(cgi)) {
        case CGI_REQ_POST:
            snprintf(tok, sizeof(tok), "%s_data_mod", uri);
            break;
        case CGI_REQ_PUT:
            snprintf(tok, sizeof(tok), "%s_data_add", uri);
            break;
        case CGI_REQ_DEL:
            snprintf(tok, sizeof(tok), "%s_data_del", uri);
            break;
        default:
        case CGI_REQ_GET:
            snprintf(tok, sizeof(tok), "%s_data_get", uri);
            break;
    }
    lses->dataer = strdup(tok);
    lses->render = strdup(uri);
    
    /* process cache */
    node = hdf_get_obj(g_cfg, PRE_CFG_FILECACHE".0");
    while (node != NULL) {
        if (reg_search(hdf_get_value(node, "uri", "NULL"), uri)) {
            lses->tm_cache_browser = hdf_get_int_value(node, "tm_cache", 0);
            break;
        }
        node = hdf_obj_next(node);
    }

    *ses = lses;
    
    return STATUS_OK;
}

void session_destroy(session_t **ses)
{
    session_t *lses;
    if (ses == NULL) return;
    lses = *ses;

    if (lses == NULL) return;

    SAFE_FREE(lses->mname);
    SAFE_FREE(lses->dataer);
    SAFE_FREE(lses->render);

    free(lses);
    lses = NULL;
}
