#include "mheads.h"
#include "lheads.h"

HDF *g_cfg = NULL;

int main(int argc, char *argv[])
{
    NEOERR *err;

    mtc_init(TC_ROOT"static");

    err = mcfg_parse_file(SITE_CONFIG, &g_cfg);
    DIE_NOK_MTL(err);

    err = ltpl_parse_dir(PATH_TPL"config/static", NULL);
    DIE_NOK_MTL(err);

    return 0;
}
