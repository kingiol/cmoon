#ifndef __LCFG_H__
#define __LCFG_H__

#include "mheads.h"

__BEGIN_DECLS

#define SITE_DOMAIN     "dididada.org"
#define IMG_URL         "http://www.dididada.org/data/"
#define IMG_ROOT        "/var/www/dida/fly/data/"
#define TC_ROOT         "/var/log/moon/dida/"

#define PATH_SITE       "/var/www/dida/"
#define SITE_CONFIG     PATH_SITE"config.hdf"
#define PATH_DOC		PATH_SITE"fly/"
#define PATH_CGI		PATH_SITE"run/"
#define PATH_TPL		PATH_SITE"tpl/"
#define PATH_MTL		PATH_SITE"mtl/"

#define PRE_REQ_IP      PRE_CGI".RemoteAddress"
#define PRE_REQ_URI     PRE_CGI".ScriptName"    /* lutil_file_access() */
#define PRE_REQ_URI_RW  PRE_QUERY".ScriptName"  /* lutil_file_access_rewrited() */
#define PRE_REQ_AJAX_FN PRE_QUERY".JsonCallback"/* main() */

#define PRE_CFG_OUTPUT  	"Output"
#define PRE_CFG_LAYOUT  	"Layout"
#define PRE_CFG_DATASET 	"Dataset"
#define PRE_CFG_REQLIMIT    "ClientReqLimit"
#define PRE_CFG_FILECACHE   "FileCache"
#define PRE_CFG_DATAER      "data_geter"

#define PRE_WALK_SACTION    "SpecialAction"

#define PRE_MMC_COUNT        "Ttnum"

#define REXP_EMAIL "^([a-zA-Z0-9_\.\-\+])+\@(([a-zA-Z0-9\-])+\.)+([a-zA-Z0-9]{2,4})+$"

__END_DECLS
#endif    /* __LCFG_H__ */
