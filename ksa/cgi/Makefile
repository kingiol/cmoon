LOCALBASE = ../
include $(LOCALBASE)Make.env
include $(BASEDIR)Make.env

CFLAGS += -rdynamic

INCS	= $(INC_LOCAL) $(INC_MOON)
LIBS	= $(LIB_LOCAL) $(LIB_MOON)

SOURCES = $(wildcard *.c)
BINARY = $(patsubst %.c, %, $(SOURCES))
DEPEND = .depend

all: preload $(BINARY)

$(DEPEND): $(SOURCES)
	@$(CC) $(CFLAGS) -MM $^ $(INCS) > $@

preload:

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
	@echo "=== "$(LIBS)
install: all
	@mkdir -p -m 775 $(PATH_FRT_CGI)
	@strip $(BINARY)
	@cp $(BINARY) $(PATH_FRT_CGI)/
	@chmod a+x $(PATH_FRT_CGI)/$(BINARY)
release: all
	@strip $(BINARY)
	@scp $(BINARY) $(USER_DEST)@$(SITE_DEST):$(PATH_DEST_CGI)
clean:
	@rm -rf ${BINARY}
distclean:
	@rm -rf $(PATH_FRT_CGI)/*
	@rm -rf ${BINARY}
