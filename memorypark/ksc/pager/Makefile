BASEDIR = ../../
include $(BASEDIR)Make.env
LOCALBASE = ../
include $(LOCALBASE)Make.env

INCS	= $(INC_MEVENT) $(INC_MNL) $(INC_MEMC) $(INC_CS) $(INC_JSON) $(INC_FCGI) $(INC_SYSTEM) $(INC_PUB)
LIBS	= $(LIB_MEVENG) $(LIB_MNL) $(LIB_MEMC) $(LIB_CS) $(LIB_JSON) $(LIB_FCGI) $(LIB_SYSTEM) $(LIB_PUB)

SOURCES = $(wildcard *.c)
BINARY = $(patsubst %.c, %, $(SOURCES))
DEPEND = .depend

all: preload $(BINARY)

$(DEPEND): $(SOURCES)
	@$(CC) $(CFLAGS) -MM $^ $(INCS) > $@

preload:
	@$(MAKE) -C $(MNLBASE)
	@$(MAKE) -C $(ROBOTBASE)

include $(DEPEND)
%:%.c
	@if [ "$<" = `ls *.c|awk '{print $1}'|sed -n '1p'` ]; then \
		echo "=== "$(CFLAGS); \
		echo "=== "$(INCS); \
		echo "=== "$(LIBS); fi
	@echo "$(CC) -o $@"
	@$(CC) $(CFLAGS) $< -o $@ $(INCS) ${LIBS}

cflag:
	@echo "=== "$(CFLAGS)
	@echo "=== "$(INCS)
generate: all
	./generateAll.sh
clean:
	@rm -rf ${BINARY}
