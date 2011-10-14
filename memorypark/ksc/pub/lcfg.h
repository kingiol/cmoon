#ifndef __LCFG_H__
#define __LCFG_H__

#include "mheads.h"

__BEGIN_DECLS

#define SITE_DOMAIN    "kossenchem.com"

#define IMG_ROOT    "../htdocs/img/"

#define IMG_ORI    "ori"
#define IMG_XXS    "24x24"
#define IMG_XS    "48x48"
#define IMG_S    "120x120"
#define IMG_M    "250x250"
#define IMG_L    "800X600"
#define IMG_XL    "1024X768"
#define IMG_XXL    "1440x900"

#ifdef RELEASE
#define PATH_SITE    "/usr/local/moon/ksc/"
#else
#define PATH_SITE    "/home/bigml/web/moon/ksc/"
#endif
#define TC_ROOT        "/var/log/moon/ksc/"
#define SITE_CONFIG        PATH_SITE"config.hdf"
#define PATH_FRT_DOC    PATH_SITE"htdocs/"
#define PATH_FRT_CGI    PATH_SITE"cgi-bin/"
#define PATH_FRT_TPL    PATH_SITE"tpl/"
#define PATH_FRT_MTLS    PATH_SITE"mtls/"
#define F_TPL_LAYOUT    PATH_FRT_TPL"layout.html"

__END_DECLS
#endif    /* __LCFG_H__ */
