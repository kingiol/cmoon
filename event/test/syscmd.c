#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "mevent.h"
#include "timer.h"

int main(int argc, char *argv[])
{
	unsigned long s_elapsed;
	mevent_t *evt;
	int ret;
    char plugin[64] = "uic";
	int cmd;
    char key[64], val[64];
    
	cmd = 100;
	
	if (argc > 1) {
		strncpy(plugin, argv[1], sizeof(plugin));
	} else {
		printf("Usage: %s [PLUGIN] [COMMAND] [key] [val]\n", argv[0]);
		return 1;
	}
	if (argc > 2) {
		cmd = atoi(argv[2]);
	}
	if (argc > 3) {
		strncpy(key, argv[3], sizeof(key));
	}
	if (argc > 4) {
		strncpy(val, argv[4], sizeof(val));
	}

	evt = mevent_init_plugin(plugin, cmd, FLAGS_SYNC);
    mevent_add_str(evt, NULL, "cachekey", key);
    mevent_add_str(evt, NULL, "cacheval", val);

    ret = mevent_trigger(evt);
    if (PROCESS_OK(ret)) {
        printf("process success %d\n", ret);
        data_cell_dump(evt->rcvdata);
    } else {
        printf("process failure %d!\n", ret);
    }

	mevent_free(evt);
	return 0;
}