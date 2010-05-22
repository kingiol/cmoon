#ifndef __LCFG_H__
#define __LCFG_H__

#include "mheads.h"

__BEGIN_DECLS

#define SITE_DOMAIN	"kosherall.com"
#define TC_ROOT		"/var/log/moon/ksc/"

#define IMG_ROOT	"../htdocs/img/"

#define IMG_ORI	"ori"
#define IMG_XXS	"24x24"
#define IMG_XS	"48x48"
#define IMG_S	"120x120"
#define IMG_M	"250x250"
#define IMG_L	"800X600"
#define IMG_XL	"1024X768"
#define IMG_XXL	"1440x900"

#ifdef RELEASE
/* #define PATH_SITE	"/usr/local/moon/www/" */
#define PATH_SITE	"/home/bigml/web/moon/"
#define SITE_CONFIG	"/home/kosheeral/www/config.hdf"
#else
#define PATH_SITE	"/home/bigml/web/moon/"
#define SITE_CONFIG	"/home/bigml/web/moon/ksc/config.hdf"
#endif
#define PATH_FRT_DOC	PATH_SITE"ksc/htdocs/"
#define PATH_FRT_CGI	PATH_SITE"ksc/cgi-bin/"
#define PATH_FRT_TPL	PATH_SITE"ksc/tpl/"
#define PATH_FRT_MTLS	PATH_SITE"ksc/mtls/"
#define PATH_ADM_DOC	PATH_SITE"admin/htdocs/"
#define PATH_ADM_CGI	PATH_SITE"admin/cgi-bin/"
#define PATH_ADM_TPL	PATH_SITE"admin/tpl/"
#define PATH_ADM_MTLS	PATH_SITE"admin/mtls/"
#define F_TPL_LAYOUT	PATH_SITE"ksc/tpl/layout.html"

#define NAV_NUM 7
extern anchor_t g_nav[NAV_NUM];

__END_DECLS
#endif	/* __LCFG_H__ */
