#include "mheads.h"
#include "lheads.h"
#include "oplace.h"

static unsigned char *ips = NULL;
static unsigned int ipbgn, ipend;

static unsigned int b2int(unsigned char *p, int count)
{
	int i;
	unsigned int ret;

	if(count < 1 || count > 4) 
		return 0;
	
	ret = p[0];
	for (i = 0; i < count; i++)
		ret |= ((unsigned int)p[i])<<(8*i);
	
	return ret;
}

static void load_ipfile()
{
	if (ips == NULL) {
		NEOERR *err = ne_load_file(PATH_MTLS"QQWry.Dat", (char**)&ips);
		RETURN_NOK(err);

		ipbgn = b2int(ips, 4);
		ipend = b2int(ips+4, 4);

		if (ipbgn == -1 || ipend == -1) {
			mtc_err("file format error");
			if (ips) free(ips);
			ips = NULL;
		}
	}
}

static unsigned char* readarea(int offset)
{
	if (!ips) return NULL;
	
	unsigned char *p = ips + offset;
	unsigned char mode = *p;
	
	if (mode == REDIRECT_MODE_1 || mode == REDIRECT_MODE_2) {
		offset = b2int(p+1, 3);
		if (offset == 0) {
			return NULL;
		}
	}
	return ips + offset;
}

static void ip_place(int offset, char **c, char **a)
{
	if (!ips) return;
	
	unsigned char *p = ips + offset + 4;
	unsigned char mode = *p;

	if (mode == REDIRECT_MODE_1) {
		offset = b2int(p+1, 3);
		p = ips + offset;
		mode = *p;
		if (mode == REDIRECT_MODE_2) {
			*c = (char*)(ips + b2int(p+1, 3));
			*a = (char*)readarea(offset+4);
		} else {
			*c = (char*)(ips + offset);
			*a = (char*)readarea(offset + strlen(*c) + 1);
		}
	} else if (mode == REDIRECT_MODE_2) {
		offset = b2int(p+1, 3);
		*c = (char*)(ips + offset);
		*a = (char*)readarea(offset+4+4);
	} else {
		*c = (char*)(ips + offset + 4);
		*a = (char*)(readarea(offset + 4 + strlen(*c) + 1));
	}
}

static int ip_offset(unsigned int ip)
{
	unsigned int ipb, ipe;
	unsigned int M, L, R, record_count;

	record_count = (ipend - ipbgn)/7+1;
	/* search for right range */
	L = 0;
	R = record_count - 1;
	while (L < R-1) {
		M = (L + R) / 2;
		ipb = b2int(ips + ipbgn + M*7, 4);
		ipe = b2int(ips + b2int(ips + ipbgn + M*7 + 4, 3), 4);

		if (ip == ipb) {
			L = M;
			break;
		}
		if (ip > ipb)
			L = M;
		else
			R = M;
	}

	ipb = b2int(ips + ipbgn + L*7, 4);
	ipe = b2int(ips + b2int(ips + ipbgn + L*7 + 4, 3), 4);

#if 0
	/* version infomation, the last item */
	if((ip & 0xffffff00) == 0xffffff00)
		set_ip_range(R, &f);
#endif
	
	if (ipb <= ip && ip <= ipe)
		return b2int(ips + ipbgn + L*7 + 4, 3);
	else
		return -1;
}

static int ip2addr_data_get(char *ip, char **c, char **a)
{
	*c = *a = NULL;
	
	if (!ip) return RET_RBTOP_INPUTE;

	char *p, *s;
	unsigned int dip = 0;

	s = strdup(ip);
	p = strtok(s, ".");
	while (p) {
		dip <<= 8;
		dip |= (unsigned int)atoi(p);
		p = strtok(NULL, ".");
	}
	free(s);

	load_ipfile();
	if (ips) {
		int offset = ip_offset(dip);
		if (offset != -1) {
			ip_place(offset, c, a);
			return RET_RBTOP_OK;
		}
	}

	return RET_RBTOP_ERROR;
}

int place_data_get(CGI *cgi, HASH *dbh, HASH *evth, session_t *ses)
{
	if (!cgi || !cgi->hdf) return RET_RBTOP_INPUTE;

	char *ip, *c, *a;
	int ret;

	HDF_GET_STR(cgi->hdf, PRE_QUERY".ip", ip);

	c = a = NULL;
	ret = ip2addr_data_get(ip, &c, &a);
	if (ret == RET_RBTOP_OK) {
		hdf_set_value(cgi->hdf, PRE_OUTPUT".a", a);
		hdf_set_value(cgi->hdf, PRE_OUTPUT".c", c);
	}
	
	return ret;
}
