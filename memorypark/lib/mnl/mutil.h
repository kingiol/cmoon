#define TGT_PARENT  "parent."
#define TGT_OPENNER "opener."
#define TGT_TOP     "top."
#define TGT_SELF    "self."

#define URL_BLANK   "location.href='/blank.html'"
#define URL_RELOAD  "location.reload()"
#define URL_CLOSE   "close()"
#define URL_BACK    "history.back(-1)"
#define URL_HREF    ""


void mutil_redirect(const char *msg, const char *target, const char *url, bool header);
