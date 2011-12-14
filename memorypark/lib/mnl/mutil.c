void mutil_redirect(const char *msg, const char *target, const char *url, bool header)
{
    char outstr[LEN_MD];
    char tok[LEN_SM];
    
    if (header) {
        printf("Content-Type: text/html; charset=UTF-8\n\n");
    }
    strcpy(outstr, "<script language='javascript'>");
    
    if (msg != NULL) {
        snprintf(tok, sizeof(tok), "alert('%s');", msg);
        strcat(outstr, tok);
    }
    
    strcat(outstr, "window.");
    
    if (target != NULL) {
        strncat(outstr, target, sizeof(tok));
    } else {
        strcat(outstr, TGT_SELF);
    }
    
    if (!strcmp(url, URL_BLANK) ||
        !strcmp(url, URL_RELOAD) ||
        !strcmp(url, URL_CLOSE) ||
        !strcmp(url, URL_BACK)) {
        strncat(outstr, url, sizeof(tok));
    } else {
        snprintf(tok, sizeof(tok), "location.href='%s'", url);
        strcat(outstr, tok);
    }
    strcat(outstr, ";</script>\n");

    printf(outstr);
}








#ifndef DROP_FCGI
int read_cb(void *ptr, char *data, int size) {
    return fread(data, sizeof(char), size, FCGI_stdin);
}
int printf_cb(void *ptr, const char *format, va_list ap) {
    return vprintf(format, ap);
}
int write_cb(void *ptr, const char *data, int size) {
    return fwrite((void *)data, sizeof(char), size, FCGI_stdout);
}
#endif
