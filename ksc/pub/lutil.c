#include "mheads.h"
#include "lheads.h"

/* make sure result has enough capacity */
int lutil_image_accept(FILE *fp, char *path, unsigned char *result)
{
	if (fp == NULL || result == NULL || path == NULL)
		return RET_RBTOP_INPUTE;

    md5_ctx my_md5;
	unsigned char data[4096], hexres[16];
	unsigned int bytes;

	memset(data, 0x0, sizeof(data));
    MD5Init(&my_md5);

	fseek(fp, 0, SEEK_SET);
	while ((bytes = fread(data, 1, 4096, fp)) != 0) {
		MD5Update(&my_md5, data, bytes);
		memset(data, 0x0, sizeof(data));
	}
	memset(hexres, 0x0, 16);
    MD5Final(hexres, &my_md5);

	mmisc_hex2str(hexres, 16, result);

	char fname[LEN_FN];
	snprintf(fname, sizeof(fname), IMG_ROOT"%s/%s/%s.jpg", path, IMG_ORI, result);
	FILE *fpout = fopen(fname, "w+");
	if (fpout == NULL) {
		mtc_err("open %s for write failure", fname);
		return RET_RBTOP_CREATEFE;
	}

	fseek(fp, 0, SEEK_SET);
	while ((bytes = fread(data, 1, 4096, fp)) != 0)
		fwrite(data, 1, bytes, fpout);
	fclose(fpout);

    /*
     * take sooo long time, so, process it backend later async
     */
#if 0
    char tok[LEN_FN];
    int ret;
    snprintf(tok, sizeof(tok), PATH_MTLS"imagemagick.sh %s %s.jpg >> /tmp/imagemagick.log 2>&1",
             path, result);
    ret = system(tok);
    if (ret != 0) {
        mtc_err("process %s failure %d", result, ret);
        return RET_RBTOP_IMGPROE;
    }
#endif

	return RET_RBTOP_OK;
}
